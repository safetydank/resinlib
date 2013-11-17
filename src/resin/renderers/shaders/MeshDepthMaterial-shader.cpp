void MeshDepthMaterial::initUniforms(UniformMap& um) {
    um = {
        { "mNear", newUniform<'f'>(1.0f) },
        { "mFar", newUniform<'f'>(2000.0f) },
        { "opacity", newUniform<'f'>(1.0f) },
    };
}

const std::string MeshDepthMaterial::kVertexShader = 
"void main() {\n"
"gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );\n"
"}\n";

const std::string MeshDepthMaterial::kFragmentShader = 
"uniform float mNear;\n"
"uniform float mFar;\n"
"uniform float opacity;\n"
"void main() {\n"
"float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
"float color = 1.0 - smoothstep( mNear, mFar, depth );\n"
"gl_FragColor = vec4( vec3( color ), opacity );\n"
"}\n";