////////////////////////////////////////////////////////////////////////////////
///
/// @file PanchitoCannon.h
/// @author Juanmi Huertas Delgado
/// @brief This class has the scene with the wall (for now)
/// This class and project has to be developed. It is meant to be a small demo
///   of something with physics and Panchito, made with OCTET Framework
///   I plan to add camera movement and probably textures.
////////////////////////////////////////////////////////////////////////////////

namespace octet {
  /// Scene containing a box with octet.
  class PanchitoCannon : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;

	// Needed for the physics simulation
	btDefaultCollisionConfiguration config;       /// setup for the world
	btCollisionDispatcher *dispatcher;            /// handler for collisions between objects
	btDbvtBroadphase *broadphase;                 /// handler for broadphase (rough) collision
	btSequentialImpulseConstraintSolver *solver;  /// handler to resolve collisions
	btDiscreteDynamicsWorld *world;             /// physics world, contains rigid bodies

	dynarray<btRigidBody*> rigid_bodies;
	dynarray<scene_node*> nodes;
	
	//Random stuff! 
	random randomGenerator;

  //Camera stuff!
  mouse_ball camera;

	void add_box(mat4t_in modelToWorld, vec3_in size, material *mat, bool is_dynamic = true) {

		btMatrix3x3 matrix(get_btMatrix3x3(modelToWorld));
		btVector3 pos(get_btVector3(modelToWorld[3].xyz()));

		btCollisionShape *shape = new btBoxShape(get_btVector3(size));

		btTransform transform(matrix, pos);

		btDefaultMotionState *motionState = new btDefaultMotionState(transform);
		btScalar mass = is_dynamic ? 1.0f : 0.0f;
		btVector3 inertiaTensor;

		shape->calculateLocalInertia(mass, inertiaTensor);

		btRigidBody * rigid_body = new btRigidBody(mass, motionState, shape, inertiaTensor);
		world->addRigidBody(rigid_body);
		rigid_bodies.push_back(rigid_body);

		mesh_box *box = new mesh_box(size);
		scene_node *node = new scene_node(modelToWorld, atom_);
		nodes.push_back(node);

		app_scene->add_child(node);
		app_scene->add_mesh_instance(new mesh_instance(node, box, mat));
	}

  public:
    /// this is called when we construct the class before everything is initialised.
	  PanchitoCannon(int argc, char **argv) : app(argc, argv) {
		  dispatcher = new btCollisionDispatcher(&config);
		  broadphase = new btDbvtBroadphase();
		  solver = new btSequentialImpulseConstraintSolver();
		  world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, &config);
    }

	  ~PanchitoCannon() {
		  delete world;
		  delete solver;
		  delete broadphase;
		  delete dispatcher;
	  }

    /// this is called once OpenGL is initialized
    void app_init() {
	    app_scene = new visual_scene();
	    app_scene->create_default_camera_and_lights();
	    app_scene->get_camera_instance(0)->get_node()->translate(vec3(0, 5, 0));
      camera.init(this, 5, 100);
	    mat4t modelToWorld;
	    material *floor_mat = new material(vec4(0, 1, 0, 1));

	    // add the ground (as a static object)
	    add_box(modelToWorld, vec3(200.0f, 0.5f, 200.0f), floor_mat, false);

	    // add the boxes (as dynamic objects)

	    // Various colors for the different bricks
	    material *mat = new material(vec4(0, 1, 1, 1));
	    material *mat2 = new material(vec4(.2, .8, .8, 1));
	    material *mat3 = new material(vec4(.3, 1, .6, 1));
	    modelToWorld.translate(-7, 1, 0);
	    // piramyd of bricks
	    int direction = 1, limit = 10;
	    for (int j = 0; j != 10; ++j){
		    for (int i = 0; i != limit; ++i){
			    // to generate randomnly the color of each brick
			    int valor = randomGenerator.get(0, 3);
			    if (valor == 0)
				    add_box(modelToWorld, vec3(0.5f), mat);
			    else if (valor == 1)
				    add_box(modelToWorld, vec3(0.5f), mat2);
			    else if (valor == 2)
				    add_box(modelToWorld, vec3(0.5f), mat3);
			    modelToWorld.translate(1.5*direction, 0, 0);
        }
        modelToWorld.translate(-2.25*direction, 1, 0);
        direction = -1 * direction;
		    --limit;
	    }
    }

    /// this is called to draw the world
	void draw_world(int x, int y, int w, int h) {
		int vx = 0, vy = 0;
		get_viewport_size(vx, vy);
		app_scene->begin_render(vx, vy);

    //Move the camera with the mouse
    camera.update(app_scene->get_camera_instance(0)->get_node()->access_nodeToParent());

		world->stepSimulation(1.0f / 30);
		for (unsigned i = 0; i != rigid_bodies.size(); ++i) {
			btRigidBody *rigid_body = rigid_bodies[i];
			btQuaternion btq = rigid_body->getOrientation();
			btVector3 pos = rigid_body->getCenterOfMassPosition();
			quat q(btq[0], btq[1], btq[2], btq[3]);
			mat4t modelToWorld = q;
			modelToWorld[3] = vec4(pos[0], pos[1], pos[2], 1);
			nodes[i]->access_nodeToParent() = modelToWorld;
		}

		// update matrices. assume 30 fps.
		app_scene->update(1.0f / 30);


		// draw the scene
		app_scene->render((float)vx / vy);
	}
  };
}
