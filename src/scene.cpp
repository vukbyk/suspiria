#include "scene.h"

#include "components.h"
#include "model.h"
#include "world.h"

Scene::Scene()//World *val)
{
//    world = val;
    importTextures();
    importMeshes();
    initEntities();
    initShaders();
}

Scene::~Scene()
{
    //    delete world;
    delete shaderMain;
    delete shaderIrradiance;
    delete shaderShadow;
    delete shaderSky;
    delete shaderProgramFBScr;
    delete shaderDebugQuad;
    delete shaderBrdf;

    delete renderQuad;
}

void Scene::initEntities()
{
    camera = CreateEntity();
    camera.addComponent<FPSEulerComponent>(btVector3(0,0,0));
    camera.addCameraComp(0.3f, 1500.0f, 45.0f);
    camera.addTransformComponent(0.0f, 2.0f, 0.0f);//For FPS benchmark 0,2,0
//    camera.addComponent(TransformComponent(Transform()));
//    auto transformCamera = &world->reg()->get<TransformComponent>(camera);
//    transformCamera->transform.setRotation(btVector3(1,0,0), -.5);


    skyCube = CreateEntity();
    skyCube.addComponent(TransformComp(Transform()));
    skyCube.addMeshComponent("sky/skycubeinv.obj");
    skyCube.addComponent(CubeMapComp(getTextureManager()->getId("reflectCube")));
//    skyCube.addCubeMapComp("skyCubeTex");


//    prepareAssetsManyCubes();
    prepareAssetsPlane();
}

void Scene::initShaders()
{
    //shaderMain
    //Todo: SkyMap connect to shader also
    const auto textureList = std::vector<std::string>(
                {"albedoMap", "normalMap", "metallicMap", "roughnessMap",
                 "aoMap", "shadowMap","skyCube"});
    shaderMain = new ShaderPBR("pbr");
    shaderMain->setUniformNamesAndIds(textureList);
    shaderMain->bind();
    shaderMain->setTextureUniforms();
//    shaderMain->uniform = new Entity(CreateEntity());
//    shaderMain->uniform->addComponent(mvpComp(shaderMain));

    // shaderIrradiance
    shaderIrradiance = new ShaderVP("cubemap.vs", "irradiance_convolution.fs");
    shaderIrradiance->setUniformNamesAndIds({"skyCube"});
    shaderIrradiance->setUniformNamesAndIds({"screenTexture"});
    shaderIrradiance->bind();
    shaderIrradiance->setTextureUniforms();

    // shaderShadow
    shaderShadow = new ShaderMVP("shadowmap");
    shaderShadow->bind();
    shaderShadow->setTextureUniforms();

    // shaderProgramSky
    shaderSky = new ShaderVP("sky.vs", "sky.fs");
    shaderSky->setUniformNamesAndIds({"skyCubeTex"});
    shaderSky->bind();
    shaderSky->setTextureUniforms();
//    shaderProgramSky->setUniformNamesAndIds({"reflectCube"});

    // framebuffer configuration
    shaderProgramFBScr = new ShaderProgram("fbscreen");
    shaderProgramFBScr->setUniformNamesAndIds({"screenTexture"});
    shaderProgramFBScr->bind();
    shaderProgramFBScr->setTextureUniforms();

    // shaderDebugQuad
    shaderDebugQuad = new ShaderProgram("debugquad");
    shaderDebugQuad->setUniformNamesAndIds({"depthMap"});
    shaderDebugQuad->bind();
    shaderDebugQuad->setTextureUniforms();

    shaderBrdf = new ShaderProgram("brdf");

    renderQuad = new Mesh();
    renderQuad = getMeshManager()->get("renderquad.obj");
}

const Entity Scene::getCamera()
{
    return camera;
}

const Entity Scene::getSkyCube()
{
    return skyCube;
}


void Scene::importTextures()
{

    getTextureManager()->load("defaultComplex.png", true);
    getTextureManager()->load("normal1x1.png", false);
    getTextureManager()->load("white.png",     false);

    getTextureManager()->load("cyborg_normal.png", true, true);
    getTextureManager()->load("cyborg_diffuse.png", true, true);
    getTextureManager()->load("bricks2_normal.jpg", false, true );
    getTextureManager()->load("brickwall_normal.jpg", false, true );
    getTextureManager()->load("brickwall.jpg", false, true);
//    getTextureManager()->load("exoskelet_Exoskelet_BaseColor.png", false);
//    getTextureManager()->load("exoskelet_Exoskelet_Normal.png", false,false);
//    getTextureManager()->load("exoalbedo.jpg", true);
    getTextureManager()->load("vulture.png",false, true);
    getTextureManager()->load("Vulture_Diffuse.alpha_normal.jpg",false , true);
//    getTextureManager()->load("Vulture_Diffuse.alpha.png",false, true);


    getTextureManager()->load("rustediron/albedo.png",false, true);
    getTextureManager()->load("rustediron/normal.png", false , true);
    getTextureManager()->load("rustediron/roughness.png");
    getTextureManager()->load("rustediron/metallic.png");
    getTextureManager()->load("rustediron/ao.png");


    std::vector<std::string> faces
    {

        "sky/right.jpg",
        "sky/left.jpg",
        "sky/top.jpg",
        "sky/bottom.jpg",
        "sky/front.jpg",
        "sky/back.jpg"

    //        "sky/stormydays_ft.tga",
    //        "sky/stormydays_bk.tga",
    //        "sky/stormydays_up.tga",
    ////        "sky/stormydays_dn.tga",
    //        "brickwall.jpg",
    //        "sky/stormydays_rt.tga",
    //        "sky/stormydays_lf.tga"

    //        "sky/arrakisday_ft.tga",
    //        "sky/arrakisday_bk.tga",
    //        "sky/arrakisday_up.tga",
    //        "sky/arrakisday_dn.tga",
    //        "sky/arrakisday_rt.tga",
    //        "sky/arrakisday_lf.tga"
    };
    //    std::vector<std::string> reflectCube
    std::vector<std::string> reflectCube
    {
        "sky/arrakisday_ft.tga",
        "sky/arrakisday_bk.tga",
        "sky/arrakisday_up.tga",
        "sky/arrakisday_dn.tga",
        "sky/arrakisday_rt.tga",
        "sky/arrakisday_lf.tga"
    };
    getTextureManager()->loadBoxTexture("skyCubeTex", faces);
//    getTextureManager()->loadBoxTexture("reflectCube", reflectCube, false);
    getTextureManager()->loadBoxTexture("reflectCube",
                                       {"sky/arrakisday_ft.tga",
                                        "sky/arrakisday_bk.tga",
                                        "sky/arrakisday_up.tga",
                                        "sky/arrakisday_dn.tga",
                                        "sky/arrakisday_rt.tga",
                                        "sky/arrakisday_lf.tga"}, false);
}

void Scene::importMeshes()
{
    getMeshManager()->loadAssimp("vulture.obj");
    getMeshManager()->loadAssimp("cubemaya.obj");
//    getMeshManager()->loadAssimp("cubeinvert.obj");
    getMeshManager()->loadAssimp("cubeinvertmini.obj");
    getMeshManager()->loadAssimp("sphare.obj");
//    getMeshManager()->loadAssimp("sky/skycube.obj");
    getMeshManager()->loadAssimp("sky/skycubeinv.obj");
    getMeshManager()->loadAssimp("cyborg.obj");
    getMeshManager()->loadAssimp("plane10x10.obj");
    getMeshManager()->loadAssimp("renderquad.obj");
}

void Scene::prepareAssetsPlane()
{
    Entity e;
    e=CreateEntity();
//    e.addTextureAlbedoNormalComp("white.png", "normal1x1.png");//"brickwall_normal.jpg");
    e.addTexturePBRComp("defaultComplex.png","normal1x1.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
    e.addMeshComponent("plane10x10.obj");
    e.addTransformComponent( 0.0f, 0.5f, 0.0f);

    e=CreateEntity();
    e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
    e.addMeshComponent("cyborg.obj");
    e.addTransformComponent( 0.0f, 2.0f, -7.0f);

    e=CreateEntity();
    e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
    e.addMeshComponent("cubemaya.obj");
    e.addTransformComponent( 3.0, 4.0f, -7.0f);

    e=CreateEntity();
    e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
    e.addMeshComponent("vulture.obj");
    e.addTransformComponent( 0.0, 1.0f, -7.0f);

    e=CreateEntity();
    e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
    e.addMeshComponent("sphare.obj");
    e.addTransformComponent( -3.0, 2.0f, -7.0f);

    e=CreateEntity();

    e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
    e.addMeshComponent("cubemaya.obj");
    e.addTransformComponent( 3.0, 1.0f, -7.0f);
//    e.addTextureAlbedoNormalComp("white.png", "normal1x1.png");//"brickwall_normal.jpg");
}

void Scene::prepareAssetsManyCubes()
{
    Entity e;

    e=CreateEntity();
    e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
    e.addMeshComponent("cyborg.obj");
    e.addTransformComponent( 0.0f, 2.0f, -7.0f);
//    e.addTextureAlbedoNormalComp("cyborg_diffuse.png", "cyborg_normal.png");//"brickwall_normal.jpg");

    //100000 = 28fpsGentoo/Suse 24dbg-53dbg (56.5 after  remove rot and pos separate ~80 fursturm
    for(int i=0; i<400; i++)
    {
        for(int j=0; j<250; j++)
        {
            e=CreateEntity();
            e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
            e.addTransformComponent( -50.0f+i*1, 0.0f, -50.0f+j*1);
            e.addMeshComponent("cubemaya.obj");
        }
    }
    e=CreateEntity();
    e.addTextureAlbedoNormalComp("white.png", "normal1x1.png");//"brickwall_normal.jpg");
    e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
    e.addMeshComponent("cubemaya.obj");
    e.addTransformComponent( 3.0, 4.0f, -7.0f);

    e=CreateEntity();
    e.addTextureAlbedoNormalComp("vulture.png", "Vulture_Diffuse.alpha_normal.jpg");
    e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
    e.addMeshComponent("vulture.obj");
    e.addTransformComponent( 0.0, 1.0f, -7.0f);

    e=CreateEntity();
    e.addTextureAlbedoNormalComp("white.png", "normal1x1.png");//"brickwall_normal.jpg");
    e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
    e.addMeshComponent("sphare.obj");
    e.addTransformComponent( -3.0, 2.0f, -7.0f);

    e=CreateEntity();
    e.addTextureAlbedoNormalComp("white.png", "normal1x1.png");//"brickwall_normal.jpg");
    e.addTexturePBRComp("rustediron/albedo.png",/*"normal1x1.png"*/ "rustediron/normal.png", "rustediron/metallic.png", "rustediron/roughness.png", "rustediron/ao.png");
    e.addMeshComponent("cubemaya.obj");
    e.addTransformComponent( 3.0, 1.0f, -7.0f);
}

