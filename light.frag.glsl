# version 330 core
// Do not use any version older than 330!

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Inputs to the fragment shader are the outputs of the same name of the vertex shader.
// Note that the default output, gl_Position, is inaccessible!
in vec3 mynormal; 
in vec4 myvertex; 

// You will certainly need this matrix for your lighting calculations
uniform mat4 modelview;

// This first defined output of type vec4 will be the fragment color
out vec4 fragColor;

uniform vec3 color;

const int numLights = 10; 
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights 
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.
// I use ambient, diffuse, specular, shininess. 
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess; 



vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor,
                   const in vec3 normal, const in vec3 halfvec,
                   const in vec4 mydiffuse, const in vec4 myspecular,
                   const in float myshininess) {

        float nDotL = dot(normal, direction)  ;
        vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0) ;

        float nDotH = dot(normal, halfvec) ;
        vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ;

        vec4 retval = lambert + phong ;
        return retval ;
}


void main (void) 
{       
    if (enablelighting) {       
        vec4 finalcolor; 

        // YOUR CODE FOR HW 2 HERE TODO
        // A key part is implementation of the fragment shader
        const vec3 eyepos = vec3(0,0,0) ;

/*
        vec3 mypos = myvertex.xyz / myvertex.w ; // Dehomogenize current location
        vec3 eyedirn = normalize(eyepos - mypos) ;

        // Compute normal, needed for shading.
        vec3 normal = normalize( mynormal ) ;

        */

        //transform mynormal and myvertex
        vec4 transfMyNormal = transpose(inverse(modelview)) * vec4(mynormal, 0);
        vec4 transfMyVertex = modelview * myvertex;

        vec3 mypos = transfMyVertex.xyz / transfMyVertex.w ; // Dehomogenize current location
        vec3 eyedirn = normalize(eyepos - mypos) ;

        // Compute normal, needed for shading.
        vec3 normal = normalize( vec3(transfMyNormal.x, transfMyNormal.y,
        transfMyNormal.z ) ) ;


/*
        // Light 0, directional
        vec3 direction0 = normalize (light0dirn) ;
        vec3 half0 = normalize (direction0 + eyedirn) ;
        vec4 col0 = ComputeLight(direction0, light0color, normal, half0, diffuse, specular, shininess) ;

        // Light 1, point
        vec3 position = light1posn.xyz / light1posn.w ;
        vec3 direction1 = normalize (position - mypos) ; // no attenuation
        vec3 half1 = normalize (direction1 + eyedirn) ;
        vec4 col1 = ComputeLight(direction1, light1color, normal, half1, diffuse, specular, shininess) ;

*/

        finalcolor = ambient + emission;


        for( int j = 0; j < numused; ++j ) {
            vec3 position;
            vec3 direction;
            if ( lightposn[j].w != 0 ) { //if point light
                // Dehomogenize current location
                position = lightposn[j].xyz / lightposn[j].w ;
                direction = normalize ( position - mypos ) ;
            }
            else { //if direction light
                direction = normalize ( vec3(lightposn[j]) ) ;
            }
            vec3 haalf = normalize (direction + eyedirn) ;
            vec4 col = ComputeLight(direction, lightcolor[j], normal, haalf,
                                            diffuse, specular, shininess) ;

            finalcolor += col; //add to light set
        }



        //if( numused == 2 ) finalcolor = vec4(0, 0, 255, 0.3);

        fragColor = finalcolor;


        // Color all pixels black for now, remove this in your implementation!
        //finalcolor = vec4(0, 0, 255, 0.3);

    } else {
        fragColor = vec4(color, 1.0f); 
    }
}
