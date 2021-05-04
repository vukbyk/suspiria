#include "glwindow.h"

#include <QApplication>
#include <QDebug>

#include "suspiria.h"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
//# include " btBulletDynamicsCommon .h"


#include <QLoggingCategory>


//#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <LinearMath/btTransform.h>
#include <LinearMath/btQuaternion.h>
#include <iostream>


void bulletTestCopyFromMain();

int main(int argc, char *argv[])
{

    struct d
    {
        int data;
        d(const int val=0):data(val){}

        operator int&() {return data;}
        operator const int&() const {return data;}
    };

    entt::registry registry;

//    entt::entity mesh[10];
//    for(int i = 0 ; i<10; i++)
//    {
//        mesh[i]= registry.create();
//        registry.emplace<btVector3>(mesh[i], btVector3(i,i,i));
//        registry.emplace<d>(mesh[i], i);
//    }
//    auto &m = registry.get<btVector3>(mesh[0]);
//    m.setX(1000);

    entt::entity mesh2 = registry.create();
    registry.emplace<btVector3>(mesh2, btVector3(2,2,2));
    registry.emplace<d>(mesh2, 6662);

//    entt::entity cam = registry.create();
//    registry.emplace<btVector3>(cam, btVector3(3,3,3));


//    auto testView = registry.view<btVector3>();
//    for(auto mesh: testView)
//    {
////        d &n = testView.get<d>(mesh);
////        std::cout << "for:  " << n.data <<"  !!!!!\n\n\n";
//        btVector3 &n = testView.get<btVector3>(mesh);
//        std::cout << "for:  " << n.x() <<"  !!!!!\n\n\n";
//    }

//    auto group = registry.group<d>(entt::get<btVector3>);
//    for(auto entity: group)
//    {
//        auto&&[vec, dn] = group.get<btVector3, d>(entity);
////        auto dn = group.get<d>(entity);
////        auto vec = group.get<btVector3>(entity);
//        vec.setY(333);
//        std::cout << "for:  " << vec.y() <<" d: " << dn <<"  !!!!!\n\n\n";
//    }
//    for(auto entity: group)
//    {
//        auto[vec, dn] = group.get<btVector3, d>(entity);
////        auto dn = group.get<d>(entity);
////        auto vec = group.get<btVector3>(entity);
//        std::cout << "for:  " << vec.y() <<" d: " << dn <<"  !!!!!\n\n\n";
//    }


    //Disable QML debug level logging
//    QLoggingCategory::setFilterRules("*.debug=false");

//    bulletTestCopyFromMain();
    QGuiApplication app(argc, argv);
    app.setApplicationName("Suspiria");
//    app.setApplicationVersion("0.1");

    QSurfaceFormat format;
    format.setDepthBufferSize(24);

    // Request OpenGL 4.3 core or OpenGL ES 3.0.
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
    {
        qDebug("Requesting 4.3 core context");
        format.setVersion(4, 3);
//        format.setProfile(QSurfaceFormat::CoreProfile); // QSurfaceFormat::CoreProfile not working
    }
    else
    {
        qDebug("Requesting 3.0 context");
        format.setVersion(3, 0);
    }

//    setSurfaceType(QWindow::OpenGLSurface);

    // Set the number of samples used for multisampling
    format.setSamples(4);
    // Turn off vertsync format.setSwapInterval(0)
    format.setSwapInterval(0);

    QSurfaceFormat::setDefaultFormat(format);

//    Suspiria engine;
    GLWindow glWindow;
    glWindow.setWidth(1024);
    glWindow.setHeight(768);
    glWindow.show();//Maximized();
    glWindow.update();

    return app.exec();
}


void bulletTestCopyFromMain()
{
//    Bullet

    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, -10, 0));

    ///-----initialization_end-----

    //keep track of the shapes, we release memory at exit.
    //make sure to re-use collision shapes among rigid bodies whenever possible!
    btAlignedObjectArray<btCollisionShape*> collisionShapes;

    ///create a few basic rigid bodies

    //the ground is a cube of side 100 at position y = -56.
    //the sphere will hit it at y = -6, with center at -5
    {
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

        collisionShapes.push_back(groundShape);

        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -56, 0));

        btScalar mass(0.);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            groundShape->calculateLocalInertia(mass, localInertia);

        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
    }

    {
        //create a dynamic rigidbody

        //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
        btCollisionShape* colShape = new btSphereShape(btScalar(1.));
        collisionShapes.push_back(colShape);

        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass(1.f);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            colShape->calculateLocalInertia(mass, localInertia);

        startTransform.setOrigin(btVector3(2, 10, 0));

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);
    }

    /// Do some simulation

    ///-----stepsimulation_start-----
    for (int i = 0; i < 150; i++)
    {
        dynamicsWorld->stepSimulation(1.f / 60.f, 10);

        //print positions of all objects
        for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
        {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
            btRigidBody* body = btRigidBody::upcast(obj);
            btTransform trans;
            if (body && body->getMotionState())
            {
                body->getMotionState()->getWorldTransform(trans);
            }
            else
            {
                trans = obj->getWorldTransform();
            }
            qDebug("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
        }
    }

    ///-----stepsimulation_end-----

    //cleanup in the reverse order of creation/initialization

    ///-----cleanup_start-----

    //remove the rigidbodies from the dynamics world and delete them
    for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            delete body->getMotionState();
        }
        dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    //delete collision shapes
    for (int j = 0; j < collisionShapes.size(); j++)
    {
        btCollisionShape* shape = collisionShapes[j];
        collisionShapes[j] = 0;
        delete shape;
    }

    //delete dynamics world
    delete dynamicsWorld;

    //delete solver
    delete solver;

    //delete broadphase
    delete overlappingPairCache;

    //delete dispatcher
    delete dispatcher;

    delete collisionConfiguration;

    //next line is optional: it will be cleared by the destructor when the array goes out of scope
    collisionShapes.clear();
//    Bullet END
}
