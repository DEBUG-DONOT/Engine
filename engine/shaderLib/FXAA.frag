#version 460 core
out vec4 fragColor;
in vec2 TexCoords;
uniform sampler2D screenTexture;
uniform sampler2D bloomBlurTexture;
float rgb2luma(vec3 color)
{
    return dot(color,vec3(0.299,0.587,0.114));//标准亮度计算
    //有优化计算，因为人眼对蓝色不敏感
    //return color.g*(0.587/0.299)+color.r;
}
float EDGE_THRESHOLD_MIN = 0.0312;
float EDGE_THRESHOLD_MAX = 0.125;
int ITERATIONS=10;
float SUBPIXEL_QUALITY=0.75;
float QUALITY[]={1.5, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0};
vec2 inverseScreenSize=1.0/textureSize(screenTexture,0).xy;
void main()
{
    //判断是否当前位置是在边缘上
    vec3 colorCenter = texture(screenTexture,TexCoords).rgb;
    // Luma at the current fragment
    float lumaCenter = rgb2luma(colorCenter);
    // Luma at the four direct neighbours of the current fragment.
    float lumaDown = rgb2luma(textureOffset(screenTexture,TexCoords,ivec2(0,-1)).rgb);
    float lumaUp = rgb2luma(textureOffset(screenTexture,TexCoords,ivec2(0,1)).rgb);
    float lumaLeft = rgb2luma(textureOffset(screenTexture,TexCoords,ivec2(-1,0)).rgb);
    float lumaRight = rgb2luma(textureOffset(screenTexture,TexCoords,ivec2(1,0)).rgb);

    // Find the maximum and minimum luma around the current fragment.
    float lumaMin = min(lumaCenter,min(min(lumaDown,lumaUp),min(lumaLeft,lumaRight)));
    float lumaMax = max(lumaCenter,max(max(lumaDown,lumaUp),max(lumaLeft,lumaRight)));

    // Compute the delta.
    float lumaRange = lumaMax - lumaMin;

    // If the luma variation is lower that a threshold (or if we are in a really dark area), we are not on an edge, don't perform any AA.
    if(lumaRange < max(EDGE_THRESHOLD_MIN,lumaMax*EDGE_THRESHOLD_MAX)){
        fragColor = vec4(colorCenter,1.0);
        return;
    }
    //当前位置在边缘上
    //判断当前边缘是vertical还是horizontal
    //方法是通过计算在水平方向上的变化量的绝对值和垂直方向上的变化量的绝对值，哪个大就是哪个方向的edge
    // Query the 4 remaining corners lumas.
    float lumaDownLeft = rgb2luma(textureOffset(screenTexture,TexCoords,ivec2(-1,-1)).rgb);
    float lumaUpRight = rgb2luma(textureOffset(screenTexture,TexCoords,ivec2(1,1)).rgb);
    float lumaUpLeft = rgb2luma(textureOffset(screenTexture,TexCoords,ivec2(-1,1)).rgb);
    float lumaDownRight = rgb2luma(textureOffset(screenTexture,TexCoords,ivec2(1,-1)).rgb);

    // Combine the four edges lumas (using intermediary variables for future computations with the same values).
    float lumaDownUp = lumaDown + lumaUp;
    float lumaLeftRight = lumaLeft + lumaRight;

    // Same for corners
    float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
    float lumaDownCorners = lumaDownLeft + lumaDownRight;
    float lumaRightCorners = lumaDownRight + lumaUpRight;
    float lumaUpCorners = lumaUpRight + lumaUpLeft;

    // Compute an estimation of the gradient along the horizontal and vertical axis.
    float edgeHorizontal =  abs(-2.0 * lumaLeft + lumaLeftCorners)  + abs(-2.0 * lumaCenter + lumaDownUp ) * 2.0    + abs(-2.0 * lumaRight + lumaRightCorners);
    float edgeVertical =    abs(-2.0 * lumaUp + lumaUpCorners)      + abs(-2.0 * lumaCenter + lumaLeftRight) * 2.0  + abs(-2.0 * lumaDown + lumaDownCorners);

    // Is the local edge horizontal or vertical ?
    bool isHorizontal = (edgeHorizontal >= edgeVertical);
    //当前像素不一定完全位于边缘上。可能上方的像素和当前像素的颜色差距很大，也可能是下方，我们要具体确定是哪个方向
    // 因此，下一步是确定与边缘方向正交的哪个方向才是真正的边缘边界。
    // 计算出边缘像素两侧的梯度，梯度最大的地方可能就是边缘边界。
    // Select the two neighboring texels lumas in the opposite direction to the local edge.
    float luma1 = isHorizontal ? lumaDown : lumaLeft;
    float luma2 = isHorizontal ? lumaUp : lumaRight;
    // Compute gradients in this direction.
    float gradient1 = luma1 - lumaCenter;
    float gradient2 = luma2 - lumaCenter;

    // Which direction is the steepest ?
    bool is1Steepest = abs(gradient1) >= abs(gradient2);

    // Gradient in the corresponding direction, normalized.
    float gradientScaled = 0.25*max(abs(gradient1),abs(gradient2));
    //现在我们知道具体是哪个方向，我们向这个方向走半个像素，刚好从像素中心走到两个像素的交界处
    //并且计算这个交界处的亮度
    // Choose the step size (one pixel) according to the edge direction.
    float stepLength = isHorizontal ? inverseScreenSize.y : inverseScreenSize.x;

    // Average luma in the correct direction.
    float lumaLocalAverage = 0.0;

    if(is1Steepest){
        // Switch the direction
        stepLength = - stepLength;
        lumaLocalAverage = 0.5*(luma1 + lumaCenter);
    } else {
        lumaLocalAverage = 0.5*(luma2 + lumaCenter);
    }

    // Shift UV in the correct direction by half a pixel.
    vec2 currentUv =TexCoords;
    if(isHorizontal){
        currentUv.y += stepLength * 0.5;
    } else {
        currentUv.x += stepLength * 0.5;
    }
    //接着我们沿着边缘前进，找到边缘的边界并计算边界的长度
    //下一步是沿着边缘的主轴进行探索。 
    //我们向两个方向移动一个像素，查询新坐标处的亮度，计算亮度相对于上一步平均亮度的变化。
    // 如果该变化大于局部梯度，则说明我们已到达该方向的边缘尽头，并停止计算。 否则，继续增加一个像素的 UV 偏移量。
    // Compute offset (for each iteration step) in the right direction.
    vec2 offset = isHorizontal ? vec2(inverseScreenSize.x,0.0) : vec2(0.0,inverseScreenSize.y);
    // Compute UVs to explore on each side of the edge, orthogonally. The QUALITY allows us to step faster.
    vec2 uv1 = currentUv - offset;
    vec2 uv2 = currentUv + offset;

    // Read the lumas at both current extremities of the exploration segment, and compute the delta wrt to the local average luma.
    float lumaEnd1 = rgb2luma(texture(screenTexture,uv1).rgb);
    float lumaEnd2 = rgb2luma(texture(screenTexture,uv2).rgb);
    lumaEnd1 -= lumaLocalAverage;
    lumaEnd2 -= lumaLocalAverage;

    // If the luma deltas at the current extremities are larger than the local gradient, we have reached the side of the edge.
    bool reached1 = abs(lumaEnd1) >= gradientScaled;
    bool reached2 = abs(lumaEnd2) >= gradientScaled;
    bool reachedBoth = reached1 && reached2;

    // If the side is not reached, we continue to explore in this direction.
    if(!reached1){
        uv1 -= offset;
    }
    if(!reached2){
        uv2 += offset;
    }
    // If both sides have not been reached, continue to explore.
    if(!reachedBoth){

        for(int i = 2; i < ITERATIONS; i++){
            // If needed, read luma in 1st direction, compute delta.
            if(!reached1){
                lumaEnd1 = rgb2luma(texture(screenTexture, uv1).rgb);
                lumaEnd1 = lumaEnd1 - lumaLocalAverage;
            }
            // If needed, read luma in opposite direction, compute delta.
            if(!reached2){
                lumaEnd2 = rgb2luma(texture(screenTexture, uv2).rgb);
                lumaEnd2 = lumaEnd2 - lumaLocalAverage;
            }
            // If the luma deltas at the current extremities is larger than the local gradient, we have reached the side of the edge.
            reached1 = abs(lumaEnd1) >= gradientScaled;
            reached2 = abs(lumaEnd2) >= gradientScaled;
            reachedBoth = reached1 && reached2;

            // If the side is not reached, we continue to explore in this direction, with a variable quality.
            if(!reached1){
                uv1 -= offset * QUALITY[i];
            }
            if(!reached2){
                uv2 += offset * QUALITY[i];
            }

            // If both sides have been reached, stop the exploration.
            if(reachedBoth){ break;}
        }
    }   
    //计算边缘长度并估计当前点在边缘的中心还是边界上
    // Compute the distances to each extremity of the edge.
    float distance1 = isHorizontal ? (TexCoords.x - uv1.x) : (TexCoords.y - uv1.y);
    float distance2 = isHorizontal ? (uv2.x -TexCoords.x) : (uv2.y -TexCoords.y);

    // TexCoords which direction is the extremity of the edge closer ?
    bool isDirection1 = distance1 < distance2;
    float distanceFinal = min(distance1, distance2);

    // Length of the edge.
    float edgeThickness = (distance1 + distance2);

    // UV offset: read in the direction of the closest side of the edge.
    float pixelOffset = - distanceFinal / edgeThickness + 0.5;
    // Is the luma at center smaller than the local average ?
    bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;

    // If the luma at center is smaller than at its neighbour, the delta luma at each end should be positive (same variation).
    // (in the direction of the closer side of the edge.)
    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != isLumaCenterSmaller;

    // If the luma variation is incorrect, do not offset.
    float finalOffset = correctVariation ? pixelOffset : 0.0;
    // Sub-pixel shifting
    // Full weighted average of the luma over the 3x3 neighborhood.
    float lumaAverage = (1.0/12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);
    // Ratio of the delta between the global average and the center luma, over the luma range in the 3x3 neighborhood.
    float subPixelOffset1 = clamp(abs(lumaAverage - lumaCenter)/lumaRange,0.0,1.0);
    float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;
    // Compute a sub-pixel offset based on this delta.
    float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;

    // Pick the biggest of the two offsets.
    finalOffset = max(finalOffset,subPixelOffsetFinal);

    // Compute the final UV coordinates.
    vec2 finalUv =TexCoords;
    if(isHorizontal){
        finalUv.y += finalOffset * stepLength;
    } else {
        finalUv.x += finalOffset * stepLength;
    }

    // Read the color at the new UV coordinates, and use it.
    vec3 finalColor = texture(screenTexture,finalUv).rgb;
    vec3 bloomColor = texture(bloomBlurTexture,TexCoords).rgb;
    fragColor = vec4(finalColor+bloomColor,1.0);
}