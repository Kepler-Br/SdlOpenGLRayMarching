#version 330

precision highp float;

uniform float iTime;
uniform vec2 iResolution;
uniform mat4 inversedProjectionView;
uniform vec3 cameraPosition;

out vec4 fragColor;

//const int numOfSpheres = 2;
//vec4 spheres[numOfSpheres];
struct Spheres
{
    int num;
    float radius[2];
    vec3 material[2];
    vec3 location[2];
};

Spheres spheres;

void init()
{
//    spheres[0] = vec4(0.0f, 0.3f, 0.3f, 0.3);
//    spheres[1] = vec4(0.0f+cos(iTime), 0.3f, 0.3f+sin(iTime), 0.3);
    spheres.num = 2;

    spheres.radius[0] = 0.3;
    spheres.material[0] = vec3(1.0f, 0.0f, 0.0f);
    spheres.location[0] = vec3(0.0f, 0.3f, 0.3f);

    spheres.radius[1] = 0.3;
    spheres.material[1] = vec3(1.0f, 0.0f, 0.0f);
    spheres.location[1] = vec3(0.0f, 0.3f, 0.3f);
}

vec3 getMaterialFromLocation(const vec3 locaton)
{

}

float getDistance(vec3 currentMarchingLocation)
{
    float sphereMin = length(currentMarchingLocation-spheres[0].xyz)-spheres[0].w;
    for(int i = 1; i < numOfSpheres; i++)
        sphereMin = min(sphereMin, length(currentMarchingLocation-spheres[i].xyz)-spheres[i].w);
//    vec3 sphereOne = vec3(0.0f, 0.3f, 0.3f);
//    float sphereOneRadius = 0.3f;
//    float sphereDist = length(currentMarchingLocation-sphereOne)-sphereOneRadius;


    float planeDistance = currentMarchingLocation.y;

    float distance = min(sphereMin, planeDistance);
    return distance;

}

float rayMarch(const vec3 rayOrigin, const vec3 rayDirection)
{
    float distanceFromOrigin = 0.0f;

    const int maxSteps = 1000;
    const float maxDistance = 100.0f;
    const float minDistanceToSurface = 0.01f;

    for(int i = 0; i < maxSteps; i++)
    {
        vec3 currentMarchingLocation = rayOrigin + rayDirection * distanceFromOrigin;
        float distanceToScene = getDistance(currentMarchingLocation);
        distanceFromOrigin += distanceToScene;
        if(distanceFromOrigin < minDistanceToSurface ||
                distanceFromOrigin > maxDistance)
            break;
    }

    return distanceFromOrigin;
}

vec3 screenToWorld()
{
    // NORMALISED DEVICE SPACE
    float x = 2.0f*gl_FragCoord.x / iResolution.x - 1;
    float y = 2.0f*gl_FragCoord.y / iResolution.y - 1;

    // HOMOGENEOUS SPACE
    vec4 screenPos = vec4(x, -y, -1.0f, 1.0f);

    vec4 worldPos = inversedProjectionView * screenPos;
    return normalize(vec3(worldPos));
}

vec3 getNormal(vec3 location)
{
    const float epsilon = 0.001; // arbitrary — should be smaller than any surface detail in your distance function, but not so small as to get lost in float precision
    float centerDistance = getDistance(location);
    float xDistance = getDistance(location + vec3(epsilon, 0, 0));
    float yDistance = getDistance(location + vec3(0, epsilon, 0));
    float zDistance = getDistance(location + vec3(0, 0, epsilon));
    vec3 normal = (vec3(xDistance, yDistance, zDistance) - centerDistance) / epsilon;
    return normalize(normal);
}

bool isVisible(const vec3 rayOrigin, const vec3 rayDirection, vec3 targetPosition)
{
    float distanceFromOrigin = 0.0f;

    const int maxSteps = 100;
    const float maxDistance = 100.0f;
    const float minDistanceToSurface = 0.01f;

    for(int i = 0; i < maxSteps; i++)
    {
        vec3 currentMarchingLocation = rayOrigin + rayDirection * distanceFromOrigin;
        float distanceToScene = getDistance(currentMarchingLocation);
        distanceFromOrigin += distanceToScene;
        if(distanceFromOrigin < minDistanceToSurface ||
                distanceFromOrigin > maxDistance)
            break;
        if(abs(length(currentMarchingLocation)-length(targetPosition)) < minDistanceToSurface )
            return true;
    }

    return false;
}

float getDiffuse(vec3 position, vec3 lightPosition)
{
    vec3 lightVector = normalize(lightPosition-position);
    vec3 pointNormal = getNormal(position);

    float diffuse = dot(pointNormal, lightVector);

    float light = rayMarch(position+pointNormal*0.01f, lightPosition);
    if(light < length(lightPosition-position))
        diffuse *= 0.8f;
    return diffuse;
}

void main(void)
{
    init();
    vec3 rayDirection = screenToWorld();
    float distance = rayMarch(cameraPosition, rayDirection);
//    vec3 normal = getNormal(rayDirection*distance+cameraPosition);
    vec3 lightPosition = vec3(0, 10+sin(iTime)*4, 0);
    float diffuse = getDiffuse(rayDirection*distance+cameraPosition, lightPosition);
    fragColor = vec4(vec3(diffuse/1.0f), 1.0f);
}
