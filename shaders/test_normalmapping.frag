// It was expressed that some drivers required this next line to function properly
precision highp float;
 
//out vec4 gl_FragColor;
uniform sampler2D tex; 
uniform sampler2D tex_normal;
in vec3 position;
uniform vec3 light;

void main(void) {
    vec3 normal = texture2D(tex_normal, gl_FragCoord.xy/512.0).xyz*2.0 - vec3(1.0, 1.0, 1.0);
    vec4 color = texture2D(tex, gl_FragCoord.xy/512.0);
    vec3 lightdir = light - position;
    
    float ambientCoeff = max(0.0, dot(normalize(lightdir), normal));
    float attenuation = min(10000.0, dot(lightdir, lightdir));
    
    //gl_FragData[1] = color / attenuation * ambientCoeff;
    gl_FragColor = vec4(position, 1.0);
}
