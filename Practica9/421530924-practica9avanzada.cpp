/*
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <chrono>
using namespace std::chrono;
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;

float toffsetTexturau;
float toffsetTexturav;
float Textura_Fija;
float Textura_Rotacion;


//animacion arco
float CartelF;
float Cartel_Rotacion;
float TiempoP;
float AnguloR;
bool Inicio_Cartel;
bool Pausar;
bool Bajando;
bool Esperar_Arriba;
bool Esperar_Abajo;
//


//animacion
float rotDragon;
bool giroCompleto;
float posicionDragon;
float velocidadDragon;
bool moviendoDerecha;
GLfloat Animacion_Cartel;

//cabezas


// rojo
float velcabeza_Roja = 0.3f; // velocidad
float tcabeza_Roja = 0.0f; // tiempo
float radCabeza_Roja = 0.5f; // radio
float altCabeza_Roja = 0.02f; // altura

// azul
float tcabeza_Azul = 0.0f; // Tiempo senoidal
float velMovimientoCabeza_Azul = 0.1f; // movimiento senoidal 
float amplitudCabeza_Azul = 0.1f; // amplitud
float freCabeza_Azul = 1.0f; // frecuencia

// cafe
float aCabeza_Cafe = -0.5f;  // Posición donde inicia
float bCabeza_Cafe = 0.01f;  
float cCabeza_Cafe = 0.20f;   // Posición inicia en Y
float tcabeza_Cafe = 0.0f; // Tiempo 
float velFactor = 0.1f; // velocidad

// blanco
float velCabeza_Blanca = 0.1f; // velocidad
float tcabeza_Blanca = 0.0f; // tiempo
float aCabeza_Blanca = 0.3f; // amplitud


// verde
float velCabeza_verde = 0.2f; // velocidad
float tcabeza_verde = 0.0f; // tiempo


// este apartado controla los puntos de la curva de bezier
glm::vec3 punto1(-0.5f, 0.20f, 0.16f); // inicio
glm::vec3 punto2(-0.3f, 0.50f, 0.20f); // control1
glm::vec3 punto3(-0.1f, 0.30f, 0.25f); // control2
glm::vec3 punto4(0.0f, 0.20f, 0.30f); // en este apartado se finaliza todo 


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;

//letras
Texture Letra;




Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Tiamat_M;

Model dragonbase;

//alas
Model aladerecha;
Model alaizquierda;

//cabezas
Model primeracabeza;
Model segundacabeza;
Model terceracabeza;
Model cuartacabeza;
Model quintacabeza;

//------MODELO DE ARCO--------
Model base_arco;
Model arco;



Skybox skybox;


//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}

// cabeza Azul
glm::vec3 Mov_Cabeza_Azul(float Tiempo_Cabeza_Azul) {
	float y = 0.48f + amplitudCabeza_Azul * sin(freCabeza_Azul * Tiempo_Cabeza_Azul);
	return glm::vec3(-0.5f, y, -0.15f);
}

// cabeza Cafe
glm::vec3 Mov_Cabeza_Cafe(float tiempo_Cafe) {
	float t = tiempo_Cafe; // Usa el tiempo acumulado
	float x = aCabeza_Cafe + bCabeza_Cafe * t * cos(t); // Movimiento en X
	float y = cCabeza_Cafe + bCabeza_Cafe * t * sin(t); // Movimiento en Y
	return glm::vec3(x, y, -0.15f); // Retorna la nueva posición, Z permanece constante
}

// cabeza Blanca
glm::vec3 Mov_Cabeza_Blanca(float tiempo_Blanca) {
	float t = tiempo_Blanca; // Usa el tiempo acumulado
	float x = -0.5f + aCabeza_Blanca * sin(t); // Movimiento en X
	float y = 0.10f + aCabeza_Blanca * sin(t) * cos(t); // Movimiento en Y
	return glm::vec3(x, y, 0.0f); // Retorna la nueva posición, Z permanece constante
}

// cabeza Roja
glm::vec3 Mov_Cabeza_Roja(float tiempo_Roja) {
	float t = tiempo_Roja; // Usa el tiempo acumulado
	float x = -0.5f + radCabeza_Roja * cos(t); // Movimiento en X (circular)
	float y = 0.26f + altCabeza_Roja * t; // Movimiento en Y (vertical)
	float z = 0.17f + radCabeza_Roja * sin(t); // Movimiento en Z (circular)
	return glm::vec3(x, y, z); // Retorna la nueva posición
}


//  cabeza verde
glm::vec3 Mov_Cabeza_verde(float tiempo_verde) {
	// Normaliza el tiempo a [0, 1] para el movimiento
	float t = fmod(tiempo_verde * velCabeza_verde, 1.0f); // Rango [0, 1
	// Cálculo de la curva de Bézier de 4 puntos
	glm::vec3 puntoA = glm::mix(punto1, punto2, t);
	glm::vec3 puntoB = glm::mix(punto2, punto3, t);
	glm::vec3 puntoC = glm::mix(punto3, punto4, t);
	glm::vec3 puntoAB = glm::mix(puntoA, puntoB, t);
	glm::vec3 puntoBC = glm::mix(puntoB, puntoC, t);
	return glm::mix(puntoAB, puntoBC, t); // Retorna la posición en la curva
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();


	//letra
	Letra = Texture("Textures/texletramario.jpg");
	Letra.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Dragon_M = Model();
	Dragon_M.LoadModel("Models/17174_Tiamat_new.obj");

	//drargon separado
	dragonbase = Model();
	dragonbase.LoadModel("Models/dragonbase.obj");

	//alas

	aladerecha = Model();
	aladerecha.LoadModel("Models/ala_derecha.obj");

	alaizquierda = Model();
	alaizquierda.LoadModel("Models/ala_izquierda.obj");

	//cabezas

	primeracabeza = Model();
	primeracabeza.LoadModel("Models/crojaprimera.obj");

	segundacabeza = Model();
	segundacabeza.LoadModel("Models/cazulsegunda.obj");

	terceracabeza = Model();
	terceracabeza.LoadModel("Models/cverdetercera.obj");

	cuartacabeza = Model();
	cuartacabeza.LoadModel("Models/cblancocuarta.obj");

	quintacabeza = Model();
	quintacabeza.LoadModel("Models/ccafequinta.obj");

	//MODELO DE ARCO
	base_arco = Model();
	base_arco.LoadModel("Models/base_arco.obj");

	arco = Model();
	arco.LoadModel("Models/cartelobj.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;

	//animaciones del dragon
	// Variables de control para el movimiento del dragón
	posicionDragon = 0.0f;  // Inicia en -20 en el eje X
	velocidadDragon = 0.04f; // Velocidad del dragon
	moviendoDerecha = true;     // El dragón comienza moviéndose a la derech
	rotDragon = 0.0f; // Rotación en grados
	giroCompleto = false; // Indica si el dragón ha realizado un giro completo


	//-----animacion del letrero 

	CartelF = 0.0f;
	Cartel_Rotacion = 0.1f;
	Inicio_Cartel = true;
	TiempoP = 0.0f;
	Pausar = false;
	Bajando = true;
	Esperar_Arriba = false;
	Esperar_Abajo = false;
	AnguloR = 0.0f;
	

	glfwSetTime(1);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{

		auto ahora = std::chrono::high_resolution_clock::now;


		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f*deltaTime;

		if (movCoche < 30.0f)
		{
			movCoche -= movOffset * deltaTime;
			//printf("avanza%f \n ",movCoche);
			rotllanta += rotllantaOffset * deltaTime;
		}

		//animacion dragon
		// Aniamcion del Dragon
		float anguloAla = 40.0f * sin(glfwGetTime() * 2.0f);

		if (moviendoDerecha)
		{
			// El dragón se mueve hacia +20 en el eje X
			posicionDragon += deltaTime * velocidadDragon;  // Ajusta 'velocidadDragon' según la velocidad deseada
			if (posicionDragon >= 20.0f)  // Si llega a +20, cambia de dirección
			{
				// Gira 180 grados en Y
				rotDragon += 180.0f;
				moviendoDerecha = false;
				giroCompleto = true; // Marca que el dragón ha girado
			}
		}
		else
		{
			// El dragón se mueve hacia -20 en el eje X
			posicionDragon -= deltaTime * velocidadDragon;
			if (posicionDragon <= -20.0f)  // Si llega a -20, cambia de dirección
			{
				// Gira de regreso a la posición inicial
				if (giroCompleto)
				{
					rotDragon -= 180.0f; // Regresa a la posición original
					giroCompleto = false; // Resetea el estado de giro
				}
				moviendoDerecha = true;
			}
		}



		//EMPIEZA ANIMACION DEL CARTEL
		Animacion_Cartel = glfwGetTime();

		if (Animacion_Cartel > 2)
		{
			if (!Pausar)
			{
				if (Bajando)
				{
					if (CartelF < 2.6f)//PARA QUE TENGA UN LIMITE DONDE TENGA QUE SUBIR 
					{
						CartelF += Cartel_Rotacion * deltaTime;
					}
					else
					{
						Pausar = true;
						TiempoP = Animacion_Cartel;
						Esperar_Arriba = true;
						AnguloR = 0.0f;
					}
				}
				else
				{
					if (CartelF > 0.3f)//PARA QUE TENGA UN LIMITE HASTA DONDE TIENE QUE BAJAR.
					{
						CartelF -= Cartel_Rotacion * deltaTime;
					}
					else
					{
						Pausar = true;
						TiempoP = Animacion_Cartel;
						Esperar_Abajo = true;
						AnguloR = 0.0f;
					}
				}
			}
			else
			{
				float tiempoTranscurrido = Animacion_Cartel - TiempoP;

				if (tiempoTranscurrido < 2.0f)
				{
					AnguloR = 180.0f * (tiempoTranscurrido / 2.0f);
				}
				else
				{
					AnguloR = 360.0f;
				}

				if (Esperar_Arriba && tiempoTranscurrido >= 2.0f)
				{
					Pausar = false;
					Esperar_Arriba = false;
					Bajando = false;
				}
				else if (Esperar_Abajo && tiempoTranscurrido >= 2.0f)
				{
					Pausar = false;
					Esperar_Abajo = false;
					Bajando = true;
				}
			}
		}

		//TERMINA ANIMACION DEL CARTEL



	


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();



		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche-50.0f, 0.5f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();


		//letrero

		//-----se empeza el arco---------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, -0.9f, 18.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		base_arco.RenderModel();

		// Cartel
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0, CartelF, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, CartelF, 0.0f));  // Mover el letrero a su posición
		//model = glm::rotate(model, glm::radians(AnguloR), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotar sobre el eje X
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arco.RenderModel();
		model = modelaux;
		// Textura de letras 

		steady_clock::time_point lastTime_Textura_Letras = steady_clock::now();
		float updateInterval_Textura_Letras = 0.15f; // Intervalo de actualización en segundos
		// Calcula el tiempo transcurrido
		steady_clock::time_point currentTime_Textura_Letras = steady_clock::now();
		float deltaTime_Textura_Letras = duration_cast<duration<float>>(currentTime_Textura_Letras - lastTime_Textura_Letras).count();

		
		if (deltaTime >= updateInterval_Textura_Letras) {
			toffsetTexturau += 0.3f; 
			if (toffsetTexturau > 1.0f)
				toffsetTexturau = 0.0f; 
			lastTime_Textura_Letras = currentTime_Textura_Letras; // reset del tiempo
		}
		toffsetTexturav = 0.000; // desplazamiento
		//para que no se desborde la variable
		if (toffsetTexturau > 1.0)
			toffsetTexturau = 0.0;

		toffset = glm::vec2(toffsetTexturau, toffsetTexturav);

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0, CartelF, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, Textura_Fija, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letra.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//-------------------------------dragon----------------------------

		//DRAGON POR SEPARADO//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posicionDragon, 5.0f + sin(glm::radians(angulovaria * 8)), 6.0f));

		// para que se pueda aplicar la rotacion del dragon 
		if (moviendoDerecha) {
			rotDragon = 180.0f; //derecha
		}
		else {
			rotDragon = 0.0f; //izquierda
		}

		model = glm::rotate(model, glm::radians(rotDragon), glm::vec3(0.0f, 1.0f, 0.0f)); // Aplica la rotación
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		/*color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dragonbase.RenderModel();
		
		//alas

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.1f, 0.8f, 0.29f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, glm::radians(anguloAla), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotación
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		/*color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aladerecha.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.1f, 0.8f, -0.56f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, glm::radians(-anguloAla), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotacion
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		/*color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alaizquierda.RenderModel();

		//cabezas
		//1
		model = glm::mat4(1.0f);
		tcabeza_Roja += 0.01f * velcabeza_Roja; // velocidad 
		glm::vec3 posicionCabeza_Roja = Mov_Cabeza_Roja(tcabeza_Roja);
		model = modelaux; 
		model = glm::translate(model, posicionCabeza_Roja); 
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		primeracabeza.RenderModel();

		//2
		model = glm::mat4(1.0f);
		tcabeza_Azul += deltaTime; 
		glm::vec3 posicionCabeza = Mov_Cabeza_Azul(tcabeza_Azul);
		model = modelaux; 
		model = glm::translate(model, posicionCabeza); // nueva posicion
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		segundacabeza.RenderModel();

		//3
		model = glm::mat4(1.0f);
		tcabeza_verde += 0.01f; // velocidad
		glm::vec3 posicionCabeza_verde = Mov_Cabeza_verde(tcabeza_verde);
		model = modelaux; 
		model = glm::translate(model, posicionCabeza_verde); 
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		terceracabeza.RenderModel();

		//4
		model = glm::mat4(1.0f);
		tcabeza_Blanca += deltaTime * velCabeza_Blanca; // velocidad
		glm::vec3 posicionCabezaBlanca = Mov_Cabeza_Blanca(tcabeza_Blanca);
		model = modelaux; 
		model = glm::translate(model, posicionCabezaBlanca); 
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f)); 
		color = glm::vec3(1.0f, 1.0f, 1.0f); //blanco
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuartacabeza.RenderModel();

		//5
		model = glm::mat4(1.0f);
		tcabeza_Cafe += deltaTime * velFactor; // tiempo velocidad
		glm::vec3 posicionCabezaCafe = Mov_Cabeza_Cafe(tcabeza_Cafe); // funcion de movi
		model = modelaux; 
		model = glm::translate(model, posicionCabezaCafe); // pos nueva
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		quintacabeza.RenderModel();






		
		/*color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		
		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.001;
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//número 1
		//toffsetnumerou = 0.0;
		//toffsetnumerov = 0.0;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();


		for (int i = 1; i<4; i++)
		{
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();

		 }

		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMesh();
		}
 

		//número cambiante 
		/*
		¿Cómo hacer para que sea a una velocidad visible? TAREA
		*/

		// Temporizador
		steady_clock::time_point lastTime_Numeros = steady_clock::now();
		float updateInterval_Numeros = 0.4f; // Intervalo de actualización en segundos
		steady_clock::time_point currentTime_Numeros = steady_clock::now();// se calcula lo que es el tiempo
		float deltaTime_Numeros = duration_cast<duration<float>>(currentTime_Numeros - lastTime_Numeros).count();

		// Actualiza toffsetnumerocambiau solo si ha pasado el intervalo
		if (deltaTime >= updateInterval_Numeros) {
			toffsetnumerocambiau += 0.25f; // SE CAMBIA LA VELOCIDAD
			if (toffsetnumerocambiau > 1.0f)
				toffsetnumerocambiau = 0.0f; // SI EXCEDE 1
			lastTime_Numeros = currentTime_Numeros; // RESET
		}

		toffsetnumerov = 0.0f;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//-------------
		//cambiar automáticamente entre textura número 1 y número 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Numero1Texture.UseTexture();
		//if
		//Numero1Texture.UseTexture();
		//Numero2Texture.UseTexture();
		
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();




		glDisable(GL_BLEND);
		
		



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
