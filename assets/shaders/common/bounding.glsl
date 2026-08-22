void    rayBoxDst(out float dstToBox, out float dstInsideBox, vec3 boundsMin, vec3 boundsMax, vec3 rayOrig, vec3 rayDir)
{
    vec3   t0 = (boundsMin - rayOrig) / rayDir;
    vec3   t1 = (boundsMax - rayOrig) / rayDir;
    vec3   tmin = min(t0, t1);
    vec3   tmax = max(t0, t1);

    float   dstA = max(max(tmin.x, tmin.y), tmin.z);
    float   dstB = min(min(tmax.x, tmax.y), tmax.z);

    dstToBox = max(0, dstA);
    dstInsideBox = max(0, dstB - dstToBox);
}
