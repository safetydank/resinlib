// Converts THREE.js shader data (uniforms, vertex and fragment sources) to C++
//
var fs = require('fs');

self = {};
eval(fs.readFileSync('./three.js') + '');

var shaderClasses = {
    'basic' : 'MeshBasicMaterial',
    'lambert' : 'MeshLambertMaterial',
    'phong' : 'MeshPhongMaterial',
    'particle_basic' : 'ParticleBasicMaterial',
    'dashed' : 'LineDashedMaterial',
    'depth' : 'MeshDepthMaterial',
    'normal' : 'MeshNormalMaterial',
    'normalmap' : '',
    'cube' : '',
    'depthRGBA' : '',
};

var writeFile = function(path, content) {
    fs.writeFile(path, content, function(err) {
        if(err) {
            console.log(err);
        } else {
            console.log('Saved file ' + path);
        }
    });
};

var cStringFromText = function(text) {
    var lines = text.split('\n');
    for (var i=0, il=lines.length; i < il; ++i) {
        var line = lines[i];
        lines[i] = '"' + line + '\\n"';
    }
    return lines.join('\n');
};

var toFloatString = function(n) {
    var str = n.toString();
    if (str.indexOf('.') < 0) {
        str += '.0';
    }
    str += 'f';

    return str;
};

var cValue = function(uniform) {
    var retval = '';
    if (uniform.value !== undefined) {
        switch (uniform.type) {
          case 'f':
            retval = toFloatString(uniform.value);
            break;
          case 'i':
            retval = uniform.value.toString();
            break;
          case 'c':
            retval = 'Color(0x'+uniform.value.getHexString()+')';
            break;
          case 'v2':
            retval = 'Vector2('+uniform.value.toArray().map(toFloatString).join(', ')+')';
            break;
          case 'v3':
            retval = 'Vector3('+uniform.value.toArray().map(toFloatString).join(', ')+')';
            break;
          case 'v4':
            retval = 'Vector4('+uniform.value.toArray().map(toFloatString).join(', ')+')';
            break;
          case 'fv':
          case 'fv1':
          case 'm4v':
          case 'tv':
          case 'v2v':
            if (uniform.value.length > 0) {
                retval = '/* XXX - non-empty array */';
            }
            break;
          case 't':
            if (uniform.value !== null) {
                retval = '/* XXX - non-null texture! */';
            }
            break;
          default:
            retval = '/*  XXX ' + uniform.type + ' */';
        };
    }

    return retval;
};

var uniformsFromShader = function(shader, className) {
    var uniforms = shader.uniforms;

    var content = '';

    content += 'void '+className+'::initUniforms(UniformMap& um) {\n';
    content += '    um = {\n';
    for (uniformName in uniforms) {
        var uniform = uniforms[uniformName];
        content += '        { "'+uniformName+'", newUniform<\''+uniform.type+'\'>' +
                   '(' + cValue(uniform) + ') },\n';
    }
    content += '    };\n';
    content += '}\n\n';

    return content;
}

var writeSource = function(shader, className) {
    var path, cstr;

    console.log('Processing class '+className);

    var content = '';

    content += uniformsFromShader(shader, className);

    content += 'const std::string ' + className + '::kVertexShader = \n';
    content += cStringFromText(shader.vertexShader) + ';';
    content += '\n\n';
    content += 'const std::string ' + className + '::kFragmentShader = \n';
    content += cStringFromText(shader.fragmentShader) + ';';

    path = className + '-shader.cpp';
    writeFile(path, content);

    //  Save original sources
    path = className + '.vert';
    writeFile(path, shader.vertexShader);
    path = className + '.frag';
    writeFile(path, shader.fragmentShader);
};

// Write each shader and uniform declaration to a new file
for (shaderId in THREE.ShaderLib) {
    console.log('Shader: ' + shaderId);

    var className = shaderClasses[shaderId];
    var shader = THREE.ShaderLib[shaderId];

    if (className.length == 0) {
        continue;
    }

    writeSource(shader, className);
}

