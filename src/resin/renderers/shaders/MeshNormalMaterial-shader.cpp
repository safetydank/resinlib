void MeshNormalMaterial::initUniforms(UniformMap& um) {
    um = {
        { "opacity", newUniform<'f'>(1.0f) },
    };
}

const std::string MeshNormalMaterial::kVertexShader = 
"varying vec3 vNormal;\n"
"#ifdef USE_MORPHTARGETS\n"
"#ifndef USE_MORPHNORMALS\n"
"uniform float morphTargetInfluences[ 8 ];\n"
"#else\n"
"uniform float morphTargetInfluences[ 4 ];\n"
"#endif\n"
"#endif\n"
"void main() {\n"
"vNormal = normalize( normalMatrix * normal );\n"
"#ifdef USE_MORPHTARGETS\n"
"vec3 morphed = vec3( 0.0 );\n"
"morphed += ( morphTarget0 - position ) * morphTargetInfluences[ 0 ];\n"
"morphed += ( morphTarget1 - position ) * morphTargetInfluences[ 1 ];\n"
"morphed += ( morphTarget2 - position ) * morphTargetInfluences[ 2 ];\n"
"morphed += ( morphTarget3 - position ) * morphTargetInfluences[ 3 ];\n"
"#ifndef USE_MORPHNORMALS\n"
"morphed += ( morphTarget4 - position ) * morphTargetInfluences[ 4 ];\n"
"morphed += ( morphTarget5 - position ) * morphTargetInfluences[ 5 ];\n"
"morphed += ( morphTarget6 - position ) * morphTargetInfluences[ 6 ];\n"
"morphed += ( morphTarget7 - position ) * morphTargetInfluences[ 7 ];\n"
"#endif\n"
"morphed += position;\n"
"#endif\n"
"vec4 mvPosition;\n"
"#ifdef USE_SKINNING\n"
"mvPosition = modelViewMatrix * skinned;\n"
"#endif\n"
"#if !defined( USE_SKINNING ) && defined( USE_MORPHTARGETS )\n"
"mvPosition = modelViewMatrix * vec4( morphed, 1.0 );\n"
"#endif\n"
"#if !defined( USE_SKINNING ) && ! defined( USE_MORPHTARGETS )\n"
"mvPosition = modelViewMatrix * vec4( position, 1.0 );\n"
"#endif\n"
"gl_Position = projectionMatrix * mvPosition;\n"
"}\n";

const std::string MeshNormalMaterial::kFragmentShader = 
"uniform float opacity;\n"
"varying vec3 vNormal;\n"
"void main() {\n"
"gl_FragColor = vec4( 0.5 * normalize( vNormal ) + 0.5, opacity );\n"
"}\n";