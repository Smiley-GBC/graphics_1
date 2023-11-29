#include "Materials.h"

Material emerald;
Material jade;
Material obsidian;
Material pearl;
Material ruby;
Material turquoise;
Material brass;
Material bronze;
Material chrome;
Material copper;
Material gold;
Material silver;

void CreateMaterials()
{
	emerald.ambient = Vector3{ 0.0215, 0.1745, 0.0215 };
	emerald.diffuse = Vector3{ 0.07568, 0.61424, 0.07568 };
	emerald.specular = Vector3{ 0.633,	0.727811, 0.633 };
	emerald.shininess = 0.6 * SPECULAR_POWER;

	jade.ambient = Vector3{ 0.135, 0.2225, 0.1575 };
	jade.diffuse = Vector3{ 0.54, 0.89, 0.63 };
	jade.specular = Vector3{ 0.316228, 0.316228, 0.316228 };
	jade.shininess = 0.1 * SPECULAR_POWER;

	obsidian.ambient = Vector3{ 0.05375, 0.05, 0.06625 };
	obsidian.diffuse = Vector3{ 1.0, 0.829, 0.829 };
	obsidian.specular = Vector3{ 0.332741, 0.328634, 0.346435 };
	obsidian.shininess = 0.3 * SPECULAR_POWER;

	pearl.ambient = Vector3{ 0.25, 0.20725, 0.20725 };
	pearl.diffuse = Vector3{ 0.18275, 0.17, 0.22525 };
	pearl.specular = Vector3{ 0.296648, 0.296648, 0.296648 };
	pearl.shininess = 0.088 * SPECULAR_POWER;

	ruby.ambient = Vector3{ 0.1745, 0.01175, 0.01175 };
	ruby.diffuse = Vector3{ 0.61424, 0.04136, 0.04136 };
	ruby.specular = Vector3{ 0.727811, 0.626959, 0.626959 };
	ruby.shininess = 0.6 * SPECULAR_POWER;

	turquoise.ambient = Vector3{ 0.1, 0.18725, 0.1745 };
	turquoise.diffuse = Vector3{ 0.396, 0.74151, 0.69102 };
	turquoise.specular = Vector3{ 0.297254, 0.30829, 0.306678 };
	turquoise.shininess = 0.1 * SPECULAR_POWER;

	brass.ambient = Vector3{ 0.329412, 0.223529, 0.027451 };
	brass.diffuse = Vector3{ 0.780392, 0.568627, 0.113725 };
	brass.specular = Vector3{ 0.992157, 0.941176, 0.807843 };
	brass.shininess = 0.21794872 * SPECULAR_POWER;

	bronze.ambient = Vector3{ 0.2125, 0.1275, 0.054 };
	bronze.diffuse = Vector3{ 0.714, 0.4284, 0.18144 };
	bronze.specular = Vector3{ 0.393548, 0.271906, 0.166721 };
	bronze.shininess = 0.2 * SPECULAR_POWER;

	chrome.ambient = Vector3{ 0.25, 0.25, 0.25 };
	chrome.diffuse = Vector3{ 0.4, 0.4, 0.4 };
	chrome.specular = Vector3{ 0.774597, 0.774597, 0.774597 };
	chrome.shininess = 0.6 * SPECULAR_POWER;

	copper.ambient = Vector3{ 0.19125, 0.0735, 0.0225 };
	copper.diffuse = Vector3{ 0.7038, 0.27048, 0.0828 };
	copper.specular = Vector3{ 0.256777, 0.137622, 0.086014 };
	copper.shininess = 0.1 * SPECULAR_POWER;

	gold.ambient = Vector3{ 0.24725, 0.1995, 0.0745 };
	gold.diffuse = Vector3{ 0.75164, 0.60648, 0.22648 };
	gold.specular = Vector3{ 0.628281, 0.555802, 0.366065 };
	gold.shininess = 0.4 * SPECULAR_POWER;

	silver.ambient = Vector3{ 0.19225, 0.19225, 0.19225 };
	silver.diffuse = Vector3{ 0.50754, 0.50754, 0.50754 };
	silver.specular = Vector3{ 0.508273, 0.508273, 0.508273 };
	silver.shininess = 0.4 * SPECULAR_POWER;
}
