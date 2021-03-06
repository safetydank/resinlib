void ParticleBasicMaterial::initUniforms(UniformMap& um) {
    um = {
        { "psColor", newUniform<'c'>(Color(0xeeeeee)) },
        { "opacity", newUniform<'f'>(1.0f) },
        { "size", newUniform<'f'>(1.0f) },
        { "scale", newUniform<'f'>(1.0f) },
        { "map", newUniform<'t'>() },
        { "fogDensity", newUniform<'f'>(0.00025f) },
        { "fogNear", newUniform<'f'>(1.0f) },
        { "fogFar", newUniform<'f'>(2000.0f) },
        { "fogColor", newUniform<'c'>(Color(0xffffff)) },
        { "shadowMap", newUniform<'tv'>() },
        { "shadowMapSize", newUniform<'v2v'>() },
        { "shadowBias", newUniform<'fv1'>() },
        { "shadowDarkness", newUniform<'fv1'>() },
        { "shadowMatrix", newUniform<'m4v'>() },
    };
}

const std::string ParticleBasicMaterial::kVertexShader = 
"uniform float size;\n"
"uniform float scale;\n"
"#ifdef USE_COLOR\n"
"varying vec3 vColor;\n"
"#endif\n"
"#ifdef USE_SHADOWMAP\n"
"varying vec4 vShadowCoord[ MAX_SHADOWS ];\n"
"uniform mat4 shadowMatrix[ MAX_SHADOWS ];\n"
"#endif\n"
"void main() {\n"
"#ifdef USE_COLOR\n"
"#ifdef GAMMA_INPUT\n"
"vColor = color * color;\n"
"#else\n"
"vColor = color;\n"
"#endif\n"
"#endif\n"
"vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );\n"
"#ifdef USE_SIZEATTENUATION\n"
"gl_PointSize = size * ( scale / length( mvPosition.xyz ) );\n"
"#else\n"
"gl_PointSize = size;\n"
"#endif\n"
"gl_Position = projectionMatrix * mvPosition;\n"
"#if defined( USE_ENVMAP ) || defined( PHONG ) || defined( LAMBERT ) || defined ( USE_SHADOWMAP )\n"
"#ifdef USE_SKINNING\n"
"vec4 worldPosition = modelMatrix * skinned;\n"
"#endif\n"
"#if defined( USE_MORPHTARGETS ) && ! defined( USE_SKINNING )\n"
"vec4 worldPosition = modelMatrix * vec4( morphed, 1.0 );\n"
"#endif\n"
"#if ! defined( USE_MORPHTARGETS ) && ! defined( USE_SKINNING )\n"
"vec4 worldPosition = modelMatrix * vec4( position, 1.0 );\n"
"#endif\n"
"#endif\n"
"#ifdef USE_SHADOWMAP\n"
"for( int i = 0; i < MAX_SHADOWS; i ++ ) {\n"
"vShadowCoord[ i ] = shadowMatrix[ i ] * worldPosition;\n"
"}\n"
"#endif\n"
"}\n";

const std::string ParticleBasicMaterial::kFragmentShader = 
"uniform vec3 psColor;\n"
"uniform float opacity;\n"
"#ifdef USE_COLOR\n"
"varying vec3 vColor;\n"
"#endif\n"
"#ifdef USE_MAP\n"
"uniform sampler2D map;\n"
"#endif\n"
"#ifdef USE_FOG\n"
"uniform vec3 fogColor;\n"
"#ifdef FOG_EXP2\n"
"uniform float fogDensity;\n"
"#else\n"
"uniform float fogNear;\n"
"uniform float fogFar;\n"
"#endif\n"
"#endif\n"
"#ifdef USE_SHADOWMAP\n"
"uniform sampler2D shadowMap[ MAX_SHADOWS ];\n"
"uniform vec2 shadowMapSize[ MAX_SHADOWS ];\n"
"uniform float shadowDarkness[ MAX_SHADOWS ];\n"
"uniform float shadowBias[ MAX_SHADOWS ];\n"
"varying vec4 vShadowCoord[ MAX_SHADOWS ];\n"
"float unpackDepth( const in vec4 rgba_depth ) {\n"
"const vec4 bit_shift = vec4( 1.0 / ( 256.0 * 256.0 * 256.0 ), 1.0 / ( 256.0 * 256.0 ), 1.0 / 256.0, 1.0 );\n"
"float depth = dot( rgba_depth, bit_shift );\n"
"return depth;\n"
"}\n"
"#endif\n"
"void main() {\n"
"gl_FragColor = vec4( psColor, opacity );\n"
"#ifdef USE_MAP\n"
"gl_FragColor = gl_FragColor * texture2D( map, vec2( gl_PointCoord.x, 1.0 - gl_PointCoord.y ) );\n"
"#endif\n"
"#ifdef ALPHATEST\n"
"if ( gl_FragColor.a < ALPHATEST ) discard;\n"
"#endif\n"
"#ifdef USE_COLOR\n"
"gl_FragColor = gl_FragColor * vec4( vColor, opacity );\n"
"#endif\n"
"#ifdef USE_SHADOWMAP\n"
"#ifdef SHADOWMAP_DEBUG\n"
"vec3 frustumColors[3];\n"
"frustumColors[0] = vec3( 1.0, 0.5, 0.0 );\n"
"frustumColors[1] = vec3( 0.0, 1.0, 0.8 );\n"
"frustumColors[2] = vec3( 0.0, 0.5, 1.0 );\n"
"#endif\n"
"#ifdef SHADOWMAP_CASCADE\n"
"int inFrustumCount = 0;\n"
"#endif\n"
"float fDepth;\n"
"vec3 shadowColor = vec3( 1.0 );\n"
"for( int i = 0; i < MAX_SHADOWS; i ++ ) {\n"
"vec3 shadowCoord = vShadowCoord[ i ].xyz / vShadowCoord[ i ].w;\n"
"bvec4 inFrustumVec = bvec4 ( shadowCoord.x >= 0.0, shadowCoord.x <= 1.0, shadowCoord.y >= 0.0, shadowCoord.y <= 1.0 );\n"
"bool inFrustum = all( inFrustumVec );\n"
"#ifdef SHADOWMAP_CASCADE\n"
"inFrustumCount += int( inFrustum );\n"
"bvec3 frustumTestVec = bvec3( inFrustum, inFrustumCount == 1, shadowCoord.z <= 1.0 );\n"
"#else\n"
"bvec2 frustumTestVec = bvec2( inFrustum, shadowCoord.z <= 1.0 );\n"
"#endif\n"
"bool frustumTest = all( frustumTestVec );\n"
"if ( frustumTest ) {\n"
"shadowCoord.z += shadowBias[ i ];\n"
"#if defined( SHADOWMAP_TYPE_PCF )\n"
"float shadow = 0.0;\n"
"const float shadowDelta = 1.0 / 9.0;\n"
"float xPixelOffset = 1.0 / shadowMapSize[ i ].x;\n"
"float yPixelOffset = 1.0 / shadowMapSize[ i ].y;\n"
"float dx0 = -1.25 * xPixelOffset;\n"
"float dy0 = -1.25 * yPixelOffset;\n"
"float dx1 = 1.25 * xPixelOffset;\n"
"float dy1 = 1.25 * yPixelOffset;\n"
"fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx0, dy0 ) ) );\n"
"if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
"fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( 0.0, dy0 ) ) );\n"
"if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
"fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx1, dy0 ) ) );\n"
"if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
"fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx0, 0.0 ) ) );\n"
"if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
"fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy ) );\n"
"if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
"fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx1, 0.0 ) ) );\n"
"if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
"fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx0, dy1 ) ) );\n"
"if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
"fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( 0.0, dy1 ) ) );\n"
"if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
"fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx1, dy1 ) ) );\n"
"if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
"shadowColor = shadowColor * vec3( ( 1.0 - shadowDarkness[ i ] * shadow ) );\n"
"#elif defined( SHADOWMAP_TYPE_PCF_SOFT )\n"
"float shadow = 0.0;\n"
"float xPixelOffset = 1.0 / shadowMapSize[ i ].x;\n"
"float yPixelOffset = 1.0 / shadowMapSize[ i ].y;\n"
"float dx0 = -1.0 * xPixelOffset;\n"
"float dy0 = -1.0 * yPixelOffset;\n"
"float dx1 = 1.0 * xPixelOffset;\n"
"float dy1 = 1.0 * yPixelOffset;\n"
"mat3 shadowKernel;\n"
"mat3 depthKernel;\n"
"depthKernel[0][0] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx0, dy0 ) ) );\n"
"depthKernel[0][1] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx0, 0.0 ) ) );\n"
"depthKernel[0][2] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx0, dy1 ) ) );\n"
"depthKernel[1][0] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( 0.0, dy0 ) ) );\n"
"depthKernel[1][1] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy ) );\n"
"depthKernel[1][2] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( 0.0, dy1 ) ) );\n"
"depthKernel[2][0] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx1, dy0 ) ) );\n"
"depthKernel[2][1] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx1, 0.0 ) ) );\n"
"depthKernel[2][2] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx1, dy1 ) ) );\n"
"vec3 shadowZ = vec3( shadowCoord.z );\n"
"shadowKernel[0] = vec3(lessThan(depthKernel[0], shadowZ ));\n"
"shadowKernel[0] *= vec3(0.25);\n"
"shadowKernel[1] = vec3(lessThan(depthKernel[1], shadowZ ));\n"
"shadowKernel[1] *= vec3(0.25);\n"
"shadowKernel[2] = vec3(lessThan(depthKernel[2], shadowZ ));\n"
"shadowKernel[2] *= vec3(0.25);\n"
"vec2 fractionalCoord = 1.0 - fract( shadowCoord.xy * shadowMapSize[i].xy );\n"
"shadowKernel[0] = mix( shadowKernel[1], shadowKernel[0], fractionalCoord.x );\n"
"shadowKernel[1] = mix( shadowKernel[2], shadowKernel[1], fractionalCoord.x );\n"
"vec4 shadowValues;\n"
"shadowValues.x = mix( shadowKernel[0][1], shadowKernel[0][0], fractionalCoord.y );\n"
"shadowValues.y = mix( shadowKernel[0][2], shadowKernel[0][1], fractionalCoord.y );\n"
"shadowValues.z = mix( shadowKernel[1][1], shadowKernel[1][0], fractionalCoord.y );\n"
"shadowValues.w = mix( shadowKernel[1][2], shadowKernel[1][1], fractionalCoord.y );\n"
"shadow = dot( shadowValues, vec4( 1.0 ) );\n"
"shadowColor = shadowColor * vec3( ( 1.0 - shadowDarkness[ i ] * shadow ) );\n"
"#else\n"
"vec4 rgbaDepth = texture2D( shadowMap[ i ], shadowCoord.xy );\n"
"float fDepth = unpackDepth( rgbaDepth );\n"
"if ( fDepth < shadowCoord.z )\n"
"shadowColor = shadowColor * vec3( 1.0 - shadowDarkness[ i ] );\n"
"#endif\n"
"}\n"
"#ifdef SHADOWMAP_DEBUG\n"
"#ifdef SHADOWMAP_CASCADE\n"
"if ( inFrustum && inFrustumCount == 1 ) gl_FragColor.xyz *= frustumColors[ i ];\n"
"#else\n"
"if ( inFrustum ) gl_FragColor.xyz *= frustumColors[ i ];\n"
"#endif\n"
"#endif\n"
"}\n"
"#ifdef GAMMA_OUTPUT\n"
"shadowColor *= shadowColor;\n"
"#endif\n"
"gl_FragColor.xyz = gl_FragColor.xyz * shadowColor;\n"
"#endif\n"
"#ifdef USE_FOG\n"
"float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
"#ifdef FOG_EXP2\n"
"const float LOG2 = 1.442695;\n"
"float fogFactor = exp2( - fogDensity * fogDensity * depth * depth * LOG2 );\n"
"fogFactor = 1.0 - clamp( fogFactor, 0.0, 1.0 );\n"
"#else\n"
"float fogFactor = smoothstep( fogNear, fogFar, depth );\n"
"#endif\n"
"gl_FragColor = mix( gl_FragColor, vec4( fogColor, gl_FragColor.w ), fogFactor );\n"
"#endif\n"
"}\n";
