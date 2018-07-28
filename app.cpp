#include<iostream>
#include<string>
#include<stdexcept>
#include<cmath>
#include<fstream>
#include<vector>
#include<cstring>
#include<cstddef>
#include<algorithm>
#include<cstdlib>


#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

//#define STATIC_GLEW

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<ktx.h>

#include"error.h"
#include"context.h"
#include"resfun.h"
#include"app.h"//declaration of app class
#include"abst.h"//declaration of abstract class
#include"conc.h"//delaration of all concrete classes

#include<stb_image.h> 


namespace N_app
{

	const float app::PI = 22.0f / 7;

	void app::init()
	{
		std::cout << "A";
		n_plane = 0.1;
		f_plane = 1000;
		fov = glm::radians(45.0f);
		view_depth = 1.5;


		background = glm::vec4(0.5, 0.5, 0.5, 1);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

	}



	void app::run()//it will act as main for opengl application 
	{

		//vp_w = (con_w = 1280);
		//vp_h = (con_h = 800);

		vp_w = (con_w = 1366);
		vp_h = (con_h = 786);

		context obj(con_w, con_h);
		glfwMakeContextCurrent(obj.get_win());

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			error("can't get proc address");

		}

		glViewport(0, 0, vp_w, vp_h);

		chk();
		init();
		startup();
		render();
	}


	void app::startup()
	{
		abst* ptr;

		ptr = new c_prog();
		ptr->pure(this);

		ptr = new c_buffer;
		ptr->pure(this);

		ptr = new c_live;
		ptr->pure(this);

		ptr = new c_model;//creates random distribution of particles
		ptr->pure(this);

		ptr = new c_texture;
		ptr->pure(this);

		ptr = new c_matrix;
		ptr->pure(this);

		glEnable(GL_DEPTH);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}


	void app::render()
	{

		bool b = false;

		while (!glfwWindowShouldClose(glfwGetCurrentContext()))
		{


			float one = 1.0f;
			glClearBufferfv(GL_COLOR, 0, glm::value_ptr(background));
			glClearBufferfv(GL_DEPTH, 0, &one);

			abst* ptr = new c_live;//develops each frame of animation
			ptr->pure(this);

			for (int i = 1; i <= 400; ++i)//draws 1 frame of animation
			{

				glUseProgram(prog[0]);
				glVertexAttribI1i(0, i);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


			}
			glfwSwapBuffers(glfwGetCurrentContext());
			glfwPollEvents();

		}


	}



}


