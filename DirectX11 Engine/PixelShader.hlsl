#define PI 3.14159265358979323846264338327950288
#define FLT_MAX 3.402823466e+38F

struct HitData
{
	float dist;
	float3 pos;
	float3 normal;

	bool success;
};

HitData HitData_Init(float dist, float3 pos, float3 normal)
{
	HitData h;

	h.dist = dist;
	h.pos = pos;
	h.normal = normal;
	h.success = true;

	return h;
}

HitData HitData_Empty()
{
	HitData h;
	h.dist = 0.0f;
	h.normal = 0;
	h.pos = 0;
	h.success = false;

	return h;
}

struct ray
{
	float3 orig;
	float3 dir;
};

float3 ray_point_at(in ray r, float length)
{
	return r.orig + length * r.dir;
}

float3 color(in ray r)
{
	return r.dir;
}

struct Material
{
	float4 diffuse;
	float4 ambient;
	float4 specular;
	float shininess;
};

struct Sphere 
{
	float4 pos;
	float radius;
	Material material;
};

HitData Sphere_intersect(in Sphere s, in ray r)
{
	float3 oc = r.orig - s.pos.xyz;


	float a = 1.0f;
	float b = 2.0f * dot(oc, r.dir);
	float c = dot(oc, oc) - s.radius * s.radius;

	float disc = b * b - 4 * a * c;

	if (disc < 0) {
		return HitData_Empty();
	}

	float dist = (-b - sqrt(disc)) / (2.0f * a);

	if (dist < 0.0f) return HitData_Empty();

	float3 p = ray_point_at(r, dist);
	float3 normal = normalize(p - s.pos.xyz);

	return HitData_Init(dist, p, normal);
}


struct Plane
{
	float4 norm;
	float4 p0;

	Material material;
};


HitData Plane_intersect(in Plane p, in ray r)
{
	float denom = dot(p.norm.xyz, r.dir);
	if (denom > 1e-3)
	{
		float3 p0r0 = p.p0.xyz - r.orig;
		float dist = dot(p0r0, p.norm.xyz) / denom;
		if (dist > 0.0f)
		{
			float3 p = ray_point_at(r, dist);
			float3 normal = float3(0.0f, 1.0f, 0.0f);

			return HitData_Init(dist, p, normal);
		}
	}

	return HitData_Empty();
}

Texture2D sviborgTex : register(t0);
Texture2D moonTex : register(t1);

Texture2D floor : register(t2);

TextureCube box : register(t3);

SamplerState samplerDef;


float4 Sphere_toPixel(in Texture2D tex, in SamplerState state, in HitData h) 
{

	float2 uv;

	uv.x = atan2(-h.normal.x, -h.normal.z) / (2 * PI) + 0.5f;
	uv.y = 0.5f - (asin(h.normal.y) / PI); // (uvw.y * 0.5f + 0.5f);

	return tex.Sample(state, uv);
}

float4 Plane_toPixel(in Plane p, in Texture2D tex, in SamplerState state, in HitData h)
{
	float2 uv = frac(h.pos.xz);
	/*
	float3 e1 = normalize(cross(p.norm.xyz, float3(1, 0, 0)));

		//If normal and (0,0,1) are parallel, change e1
	if (!any(e1))
	{
		e1 = normalize(cross(p.norm.xyz, float3(0, 0, 1)));
	}

	float3 e2 = normalize(cross(p.norm.xyz, e1));

	float3 scaled = frac(h.pos);

	uv.x = (dot(e1, scaled) + 1.0f) / 2.0f;
    uv.y = (dot(e2, scaled) + 1.0f) / 2.0f;
	*/
	return tex.Sample(state, uv);

}

float4 Cube_toPixel(in SamplerState state, in TextureCube box, in float3 normal)
{
	return box.Sample(state, normal);
}

struct Light
{
	float4 dir;
	float4 pos;
	float intensity;
	float cone;
	float4 color;
};

cbuffer ConstantBuffer
{
	int screenW;
	int screenH;

	float time;

	int frametime;

	Sphere sp;

	Plane plane;

	Sphere moon;

	Light moon_light;

	Light pointL;

	Sphere pointS;
	
	float4 cameraPos;
	
	bool skymode;
	
	float4 skyColor1;
	float4 skyColor2;
	float4 fogColor;
	
	bool fog;
	
	row_major matrix invVP;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

ray getRay(in float2 uv, in row_major matrix transform, float3 camera)
{
	float u = uv.x;
	float v = 1.0 - uv.y;

	float4 far = mul(float4(u * 2.0 - 1.0, v * 2.0 - 1.0, 1.0, 1.0), transform);
	far /= far.w;

	ray r;
	r.orig = camera;
	r.dir = normalize(far.xyz - camera);

	return r;
}


float4 ApplyShadow(in HitData hit, in Material material, in float3 camera,
				   in Light light, in float4 color)
{
	// ambient
	float4 ambient = material.ambient * color;
	// diffuse
	float3 lightDir = light.dir.xyz;
	float3 normal = hit.normal;
	
	float distance = max(length(light.pos.xyz - hit.pos), 1e-7);

	distance *= distance;
	

	float diff = max(dot(lightDir, normal), 0.0f);
	float4 diffuse = diff * (light.intensity / distance) * color;
	return (ambient + diffuse) / 2;
}

	float4 ApplyLighting(in HitData hit, in Material material, in float3 camera,
						 in Light light, in float4 color)
{
	// ambient
	float4 ambient = material.ambient * color;
	// diffuse
	float3 lightDir = light.dir.xyz;
	float3 normal = hit.normal;
	
	float distance = max(length(light.pos.xyz - hit.pos), 1e-7);
	
	distance *= distance;

	float diff = max(dot(lightDir, normal), 0.0f);

	float4 diffuse = diff * material.diffuse * ((light.intensity / distance) * light.color) * color;
	
	
	// specular
	float3 viewDir = normalize(camera - hit.pos);
	float3 reflectDir = reflect(lightDir, normal);
	
	// blinn-phong
	float spec = 0.0;
	const float energyConservation = (8.0 + material.shininess) / (8.0 * PI);
	float3 halfwayDir = normalize(viewDir + lightDir);
	spec = energyConservation * pow(max(0.0f, dot(normal, halfwayDir)), material.shininess);
	
	float4 specular = float4(0.3, 0.3, 0.3, 0.3) * light.color * spec * (light.intensity / distance) * material.specular;
	return ambient + diffuse + specular;
}

float4 render(in PSInput input) 
{
	ray r = getRay(input.uv, invVP, cameraPos.xyz);
	float4 color = float4(0, 0, 0, 0);
	
	float4 sphereColor = float4(1, 1, 1, 1);
	float4 moonColor = float4(1, 1, 1, 1);
	float4 planeColor = float4(1, 1, 1, 1);
	
	Material sviborgMat = sp.material;
	Material planeMat = plane.material;
	sviborgMat.ambient = box.SampleLevel(samplerDef, normalize(float3(0, 1, 0)), FLT_MAX);
	planeMat.ambient = sviborgMat.ambient;
	
	HitData h = Sphere_intersect(sp, r);
	if (h.success){
		sphereColor = Sphere_toPixel(sviborgTex, samplerDef, h);
		color = ApplyLighting(h, sviborgMat, r.orig, moon_light, sphereColor);
		color += ApplyLighting(h, sviborgMat, r.orig, pointL, sphereColor);
	}
	else if ((h = Sphere_intersect(moon, r)).success) {
		moonColor = Sphere_toPixel(moonTex, samplerDef, h);
		color = moonColor;
	}
	else if ((h = Sphere_intersect(pointS, r)).success)
	{
		color = pointS.material.ambient;
	}
	else if ((h = Plane_intersect(plane, r)).success)
	{
		planeColor = Plane_toPixel(plane, floor, samplerDef, h);
		
		ray r2;
		
		r2.orig = h.pos;
		r2.dir = normalize(pointL.pos.xyz - h.pos);
		
		ray r3;
        r3.orig = h.pos;
		r3.dir = normalize(moon_light.pos.xyz - h.pos);
		
		HitData hit2 = Sphere_intersect(sp, r2);

		if (hit2.success)
		{
		    color = ApplyShadow(h, planeMat, r.orig, pointL, planeColor);
			color += ApplyShadow(h, planeMat, r.orig, moon_light, planeColor);
			color /= 2;
		}
		else
		{
			hit2 = Sphere_intersect(sp, r3);
		
			if (hit2.success)
			{
				color = ApplyShadow(h, planeMat, r.orig, pointL, planeColor);
				color += ApplyShadow(h, planeMat, r.orig, moon_light, planeColor);
				color /= 2;
			}
			else
			{
				color = ApplyLighting(h, planeMat, r.orig, moon_light, planeColor);
				color += ApplyLighting(h, planeMat, r.orig, pointL, planeColor);
			}
		}
	} else {
		if (skymode) {
			color = Cube_toPixel(samplerDef, box, r.dir);
		} else {
			color = lerp(skyColor1, skyColor2, (r.dir.y + 1.0f) / 2);
		}
		h.dist = 5.0f;
	}
	
	h.dist = max(h.dist, 1.0f);
	
	if (fog)
		color = lerp(color, fogColor, (1.0f - 1.0f / h.dist) / 2);
	
	return color;
}

float4 PSmain(PSInput input): SV_Target
{
	return render(input);
}
