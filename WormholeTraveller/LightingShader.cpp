#include "stdafx.h"
#include "LightingShader.h"


LightingShader::LightingShader()
{
}


LightingShader::~LightingShader()
{
}

OpStatus LightingShader::init()
{
	OpStatus status;
	
	status = this->createShaderProgram("LightingShader.vert", "LightingShader.frag");
	
	if (OPS_FAILURE(status)) { return status; }

	ulLightPosition = glGetUniformLocation(getProgId(), "light.position");
	ulLightAmbientInt = glGetUniformLocation(getProgId(), "light.ambientIntensity");
	ulLightDiffuseInt = glGetUniformLocation(getProgId(), "light.diffuseIntensity");
	ulLightSpecularInt = glGetUniformLocation(getProgId(), "light.specularIntensity");

	ulMatAmbientReflect = glGetUniformLocation(getProgId(), "material.ambientReflectivity");
	ulMatDiffuseReflect = glGetUniformLocation(getProgId(), "material.diffuseReflectivity");
	ulMatSpecularReflect = glGetUniformLocation(getProgId(), "material.specularReflectivity");
	ulMatShininess = glGetUniformLocation(getProgId(), "material.shininess");

	return OPS_OK;
}

void LightingShader::setLight(const DirectionalLight l)
{
	glUniform4f(ulLightPosition, l.position.x, l.position.y, l.position.z, l.position.w);
	glUniform3f(ulLightAmbientInt, l.ambientInt.x, l.ambientInt.y, l.ambientInt.z);
	glUniform3f(ulLightDiffuseInt, l.diffuseInt.x, l.diffuseInt.y, l.diffuseInt.z);
	glUniform3f(ulLightSpecularInt, l.specularInt.x, l.specularInt.y, l.specularInt.z);
}

void LightingShader::setMaterial(const Material m)
{
	glUniform3f(ulMatAmbientReflect, m.ambientReflect.x, m.ambientReflect.y, m.ambientReflect.z);
	glUniform3f(ulMatDiffuseReflect, m.diffuseReflect.x, m.diffuseReflect.y, m.diffuseReflect.z);
	glUniform3f(ulMatSpecularReflect, m.specularReflect.x, m.specularReflect.y, m.specularReflect.z);
	glUniform1f(ulMatShininess, m.shininess);
}
