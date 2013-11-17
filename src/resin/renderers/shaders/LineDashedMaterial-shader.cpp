void LineDashedMaterial::initUniforms(UniformMap& um) {
    um = {
        { "diffuse", newUniform<'c'>(Color(0xeeeeee)) },
        { "opacity", newUniform<'f'>(1.0f) },
        { "map", newUniform<'t'>() },
        { "offsetRepeat", newUniform<'v4'>(Vector4(0.0f, 0.0f, 1.0f, 1.0f)) },
        { "lightMap", newUniform<'t'>() },
        { "specularMap", newUniform<'t'>() },
        { "envMap", newUniform<'t'>() },
        { "flipEnvMap", newUniform<'f'>(-1.0f) },
        { "useRefract", newUniform<'i'>(0) },
        { "reflectivity", newUniform<'f'>(1.0f) },
        { "refractionRatio", newUniform<'f'>(0.98f) },
        { "combine", newUniform<'i'>(0) },
        { "morphTargetInfluences", newUniform<'f'>(0.0f) },
        { "fogDensity", newUniform<'f'>(0.00025f) },
        { "fogNear", newUniform<'f'>(1.0f) },
        { "fogFar", newUniform<'f'>(2000.0f) },
        { "fogColor", newUniform<'c'>(Color(0xffffff)) },
        { "scale", newUniform<'f'>(1.0f) },
        { "dashSize", newUniform<'f'>(1.0f) },
        { "totalSize", newUniform<'f'>(2.0f) },
    };
}

const std::string LineDashedMaterial::kVertexShader = 
"uniform float scale;\n"
"attribute float lineDistance;\n"
"varying float vLineDistance;\n"
"#ifdef USE_COLOR\n"
"varying vec3 vColor;\n"
"#endif\n"
"void main() {\n"
"#ifdef USE_COLOR\n"
"#ifdef GAMMA_INPUT\n"
"vColor = color * color;\n"
"#else\n"
"vColor = color;\n"
"#endif\n"
"#endif\n"
"vLineDistance = scale * lineDistance;\n"
"vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );\n"
"gl_Position = projectionMatrix * mvPosition;\n"
"}\n";

const std::string LineDashedMaterial::kFragmentShader = 
"uniform vec3 diffuse;\n"
"uniform float opacity;\n"
"uniform float dashSize;\n"
"uniform float totalSize;\n"
"varying float vLineDistance;\n"
"#ifdef USE_COLOR\n"
"varying vec3 vColor;\n"
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
"void main() {\n"
"if ( mod( vLineDistance, totalSize ) > dashSize ) {\n"
"discard;\n"
"}\n"
"gl_FragColor = vec4( diffuse, opacity );\n"
"#ifdef USE_COLOR\n"
"gl_FragColor = gl_FragColor * vec4( vColor, opacity );\n"
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