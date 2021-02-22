#include <Windows.h>
#include "Window.h"
#include "D3D11.h"
#include<DirectXMath.h>
#include "Camera.h"
#include "Mesh.h"
#include "Time.h"
#include "Light.h"
#include "Material.h"
#include "GameObject.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	//Create the Window

	INT width = 1600;
	INT height = 800;
	bool isWindowed = true;

	Window window;
	int err = window.init(hInstance, width, height, nCmdShow);
	window.initMouse();
	if (err != 0) return err;

	// Create the D3D11 interface
	D3D11 d3d11;
	err = d3d11.init(window.getWindowHandle(), width, height, isWindowed);
	if (err != 0) return err;

	// Create mesh
	Mesh mesh_Cube;
	err = mesh_Cube.init(d3d11.getDevice(),Mesh::Object::Cube);
	if (err != 0) return err;
	Mesh mesh_Floor;
	err = mesh_Floor.init(d3d11.getDevice(), Mesh::Object::Floor);
	if (err != 0) return err;
	Mesh mesh_Wall;
	err = mesh_Wall.init(d3d11.getDevice(), Mesh::Object::Wall);
	if (err != 0) return err; 

	// Create Camera
	Camera camera;
	err = camera.init(width, height);
	if (err != 0) return err;
	camera.SetPosition(0, 0, -2);

	// Create time
	Time time;
	err = time.init();
	camera.SetTime(&time);

	// Create ShaderParameters
	ShaderParameters param_Floor; 
	param_Floor.Tiling = { 4,4 };
	param_Floor.Offset = { 0,0 };
	param_Floor.VertexShaderName = L"PhongVertexShader.hlsl";
	param_Floor.PixelShaderName = L"PhongPixelShader.hlsl";
	param_Floor.SpecularPower = { 32,0 }; // parameters for rough material
	param_Floor.Glossiness = { 0.2f,0 };
	ShaderParameters param_Cubes;
	param_Cubes.Tiling = { 1,1 };
	param_Cubes.Offset = { 64,0 };
	param_Cubes.VertexShaderName = L"PhongVertexShader.hlsl";
	param_Cubes.PixelShaderName = L"PhongPixelShader.hlsl";
	param_Cubes.SpecularPower = {64,0};
	param_Cubes.Glossiness = {0.5,0};
	ShaderParameters param_Wall;
	param_Wall.Tiling = { 5,1 };
	param_Wall.Offset = { 0,0 };
	param_Wall.VertexShaderName = L"PhongVertexShader.hlsl";
	param_Wall.PixelShaderName = L"PhongPixelShader.hlsl";
	param_Wall.SpecularPower = { 64,0 };
	param_Wall.Glossiness = { 0.5,0 };
	// Create materials
	Material material_Wall;
	err = material_Wall.init(d3d11.getDevice(), L"WallAlbedo.jpg",&param_Wall);
	if (err != 0) return err;

	Material material_Cube;
	err = material_Cube.init(d3d11.getDevice(), L"SadPepeFace.png" ,&param_Cubes);
	if (err != 0) return err;

	Material material_Floor;
	err = material_Floor.init(d3d11.getDevice(), L"Wall.jpg", &param_Floor);
	if (err != 0) return err;

	

	// Create gameobject
	GameObject gameobject_Cube;
	err = gameobject_Cube.init( &material_Cube, &mesh_Cube);
	if (err != 0) return err;
	gameobject_Cube.SetScale(0.5f, 0.5f, 0.5f);

	GameObject gameobject_Cube_2;
	err = gameobject_Cube_2.init( &material_Cube, &mesh_Cube);
	if (err != 0) return err;
	gameobject_Cube_2.setPosition(1, 0, 0);
	gameobject_Cube_2.SetScale(0.5f, 0.5f, 0.5f);

	GameObject gameobject_Floor;
	err = gameobject_Floor.init(&material_Floor, &mesh_Floor);
	if (err != 0) return err;
	gameobject_Floor.setPosition(0, -1, 0);
	gameobject_Floor.SetScale(10,10,10);

	GameObject gameobject_Wall_1;
	err = gameobject_Wall_1.init(&material_Wall, &mesh_Wall);
	if (err != 0) return err;
	gameobject_Wall_1.setPosition(-4, -0.5f, 0);
	gameobject_Wall_1.SetScale(2, 2, 10);

	GameObject gameobject_Wall_2;
	err = gameobject_Wall_2.init(&material_Wall, &mesh_Wall);
	if (err != 0) return err;
	gameobject_Wall_2.setPosition(2, -0.5f, 0);
	gameobject_Wall_2.SetScale(4, 2, 10);
	gameobject_Wall_2.setRotation(0, 180*(XM_PI/180), 0);

	GameObject gameobject_Wall_3;
	err = gameobject_Wall_3.init(&material_Wall, &mesh_Wall);
	if (err != 0) return err;
	gameobject_Wall_3.setPosition(0, -0.5f, 4);
	gameobject_Wall_3.SetScale(2, 2, 10);
	gameobject_Wall_3.setRotation(0, 90 * (XM_PI / 180), 0);

	GameObject gameobject_Wall_4;
	err = gameobject_Wall_4.init(&material_Wall, &mesh_Wall);
	if (err != 0) return err;
	gameobject_Wall_4.setPosition(0, -0.5f, -4);
	gameobject_Wall_4.SetScale(2, 2, 10);
	gameobject_Wall_4.setRotation(0, -90 * (XM_PI / 180), 0);

	// Create Light
	Light light = {};
	light.LightDirection = { 0.0f, 1.0f, 0.5f };
	light.AmbientColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	light.DiffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	light.LightIntensity = 1.0f;
	material_Wall.setLight(d3d11.getDeviceContext(), light);
	material_Cube.setLight(d3d11.getDeviceContext(), light);
	material_Floor.setLight(d3d11.getDeviceContext(), light);

	while (true)
	{
		if (!window.run()) break;

		//Input update
		window.Update();

		// update objects
		time.update();

		gameobject_Cube.AdjustRotation(0.5 * time.getDeltaTime(), 0.5 * time.getDeltaTime(), 0.5 * time.getDeltaTime());
		gameobject_Cube_2.AdjustRotation(-0.5 * time.getDeltaTime(), -0.5 * time.getDeltaTime(), -0.5 * time.getDeltaTime());

		gameobject_Cube.update(time.getDeltaTime());
		gameobject_Cube_2.update(time.getDeltaTime());
		gameobject_Floor.update(time.getDeltaTime());
		gameobject_Wall_1.update(time.getDeltaTime());
		gameobject_Wall_2.update(time.getDeltaTime());
		gameobject_Wall_3.update(time.getDeltaTime());
		gameobject_Wall_4.update(time.getDeltaTime());

		camera.Update();
		
		// draw objects
		d3d11.startScene();

		// rendering

		gameobject_Cube.render(d3d11.getDeviceContext(), &camera);
		gameobject_Cube_2.render(d3d11.getDeviceContext(), &camera);
		gameobject_Floor.render(d3d11.getDeviceContext(), &camera);
		gameobject_Wall_1.render(d3d11.getDeviceContext(), &camera);
		gameobject_Wall_2.render(d3d11.getDeviceContext(), &camera);
		gameobject_Wall_3.render(d3d11.getDeviceContext(), &camera);
		gameobject_Wall_4.render(d3d11.getDeviceContext(), &camera);

		d3d11.endScene();
	}

	// Deinitialisation
	material_Wall.deInit();
	material_Cube.deInit();
	material_Floor.deInit();
	time.deInit();
	camera.deInit();
	mesh_Cube.deInit();
	d3d11.deInit();
	window.deInit();
	return 0;
}