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

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<ktx.h>

#include"error.h"
#include"context.h"
#include"resfun.h"
#include"app.h"
#include"abst.h"
#include"conc.h"

#include<stb_image.h> 

//This is Source file for definition of members of concrete classes

namespace N_app
{

	void c_prog::pure(app*th)
	{

		vert_shad = glCreateShader(GL_VERTEX_SHADER);
		frag_shad = glCreateShader(GL_FRAGMENT_SHADER);
		chk();
		(th->prog)[0] = glCreateProgram();

		std::string vert_s, frag_s;

		load_shad("Vertex.txt", vert_shad);
		load_shad("Frag.txt", frag_shad);


		glCompileShader(vert_shad);
		check_compile(vert_shad, "vert");

		glCompileShader(frag_shad);
		check_compile(frag_shad, "frag");
		chk();

		glAttachShader((th->prog)[0], vert_shad);
		glAttachShader((th->prog)[0], frag_shad);
		chk();

		glDeleteShader(vert_shad);
		glDeleteShader(frag_shad);


		chk();

		glLinkProgram((th->prog)[0]);
		check_link((th->prog)[0], "prog");
	}



	void c_buffer::pure(app*th)
	{
		glGenBuffers(2, th->buff);
	}


	void c_model::pure(app*th)
	{
		glm::vec3 sign;
		std::vector<float>depth(400);


		for (int i = 0; i < 400; ++i)
		{
			std::srand(15000 - i);
			depth[i] = (rand() % 5) + ((rand() % 99) / 100.0f);
		}

		std::sort(depth.begin(), depth.end());//sorting of z is done because we want particles with bigger z (i.e at more depth) to stay behind those with samller depth(closer to observer)
											  //so z coord are sorted in ascending order and later they are put in model_obj in decending order (by using (200-1)-i as index of depth )


		for (int i = 0; i < 400; ++i)
		{
			std::srand(15000 - i);

			float rf_x = (rand() % 2) + ((rand() % 99) / 100.0f);//particle scatter will be of the dimention 4 by 8 (because we have restricted x to 2 at max and y to be 4 at max and they can also be -ve, so overall spread will be y in [-4,4] and x in [-2,2] 
			float rf_y = (rand() % 4) + ((rand() % 99) / 100.0f);

			sign = glm::vec3(rand() % 2 ? -1 : 1, rand() % 2 ? -1 : 1, -1);//if invoked random number is even then condition will be false(as rand()%2 will be 0) and 1 will be used , otherwise
																		   //if number is odd then condition will be true and -1 will be used, z componenent will always be -1 (to keep things in visible axis), y component wll 

			th->model_obj[i].offset =( th->live_obj[i].offset = sign * glm::vec3(rf_x, rf_y, depth[(400 - 1) - i]) );//first particles with more depth are stored and then those with smaller depth, this is done because in each frame particles that are stored first will be drawn first 
																												  //and particles which are stored later will be drawn later and if there is an intersection of position then particles stored 

			th->model_obj[i].scale =( th->live_obj[i].scale = glm::vec3(2) );

			float rf_ang = (float(rand() % 361));

			th->model_obj[i].ang = th->live_obj[i].ang = rf_ang;

		}


	}

	void c_live::pure(app*th)
	{

		float t = glfwGetTime();

		//int i = 15;
		for (int i = 0; i < 400; ++i)//live action on each single frame(each i represents any particular particle in frame )
		{
			th->live_obj[i].scale = th->live_obj[i].scale;


			th->live_obj[i].offset = th->live_obj[i].offset + glm::vec3(0, -0.001, 0);


			th->live_obj[i].ang = th->live_obj[i].ang + glm::radians(-0.1);


			float y = th->live_obj[i].offset.y;
			float z = th->live_obj[i].offset.z;

			float max_proj_y = th->get_max_proj_y();//max y coord of near plane
			float proj_y = th->get_proj_y(y, z, max_proj_y);//y coord on near plane corrosponding to actual y coord 

			static int count = 0;
			static float horz_shift[3] = { -1,0,1 };

			if (proj_y < -1 * (max_proj_y + 0.01)) //if y coord (projected on near plane) gets past the bottommost part of near plane (i.e y==-*max_proj_y) then for next frame put it back at some position on top (which is not yet visible(so that particle dosn't come out of nowhere in frame) )
												   //although if we do this at -1*max_proj_y then it will vanish as soon as touching bottommost part of screen which is unnatural so do this after -1*(max_proj_y + 0.01) 
			{

				th->live_obj[i].offset = th->model_obj[i].offset + glm::vec3(horz_shift[count % 3], 8, 0);

			}
			++count;

		}


		glBindBuffer(GL_UNIFORM_BUFFER, th->buff[0]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(th->live_obj), &(th->live_obj), GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 3, th->buff[0]);

	}


	void c_texture::pure(app*th)
	{

		glGenSamplers(1, &th->sam);
		glSamplerParameteri(th->sam, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameteri(th->sam, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindSampler(0, th->sam);

		glGenTextures(1, &th->tex);
		glBindTexture(GL_TEXTURE_2D_ARRAY, th->tex);
		glActiveTexture(GL_TEXTURE0);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 10, GL_RGBA8, 900, 892, 5);
		chk();

		int w, h, c;
		const char* pic_name[] = { "leaf1.png","leaf2.png","leaf3.png","leaf4.png","leaf5.png" };

		for (int i = 0; i < 5; ++i)
		{
			unsigned char* data = stbi_load(pic_name[i], &w, &h, &c, 0);
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, w, h, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
			chk();
		}


	}

	void c_matrix::pure(app*th)
	{


		th->matrix_obj.M_proj = glm::perspective(th->fov, (float(th->vp_w) / float(th->vp_h)), th->n_plane, th->f_plane);
		th->matrix_obj.M_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -1 * th->view_depth));

		glBindBuffer(GL_UNIFORM_BUFFER, th->buff[1]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(th->matrix_obj), &(th->matrix_obj), GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 4, th->buff[1]);

	}

};