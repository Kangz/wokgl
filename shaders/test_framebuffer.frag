// It was expressed that some drivers required this next line to function properly
precision highp float;
 
//out vec4 gl_FragColor;
uniform sampler2D rtt; 
in vec3 position;

void main(void){
    vec4 color = texture2D(rtt, gl_FragCoord.xy/512.0);
    
    gl_FragColor = vec4(1.0) - color;
}
