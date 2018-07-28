namespace N_app
{


	class app
	{

	protected:
		int con_w, con_h, vp_w, vp_h;
		float n_plane, f_plane,fov;
		float view_depth;//-z coord put in view matrix
		GLuint prog[1];
		GLuint vao;
		
		GLuint buff[2];
		GLuint tex;
		GLuint sam;


		struct st_live//application counterpart of structure st_live
		{
			glm::vec3 offset;
			float pad1;

			glm::vec3 scale;
			float pad2;
			
			float ang;
			float pad3;
			float pad4;
			float pad5;

		}live_obj[400],model_obj[400];



		struct st_matrix//application counterpart of structure st_matrix
		{
			glm::mat4 M_proj;
			glm::mat4 M_view;
		//	glm::mat4 M_tr;
		}matrix_obj;




		glm::vec4 background;

	public:
		static const float PI;
		
		//app();
		~app()
		{
			glDeleteProgram(prog[0]);
			glDeleteVertexArrays(1, &vao);

		}
		void render();
		void startup();//for taskes and computations just before rendering
		void init();
		void run();
		inline float get_max_proj_y() { return n_plane*tan(fov / 2.0f); } //returns max y coord of near plane(near plane is also called projection plane)
		inline float get_proj_y(float y, float z,float max_proj_y)  //returns projected y coord( on near plane) corrosponding to actual y
		{                 
			z = z - view_depth;//since view trasformation happens later so we need to substract by view_depth (because in view we are sending particles view_depth units forward(in -ve z axis) to imitate its effects
			return -1 * (n_plane*(y / z));
		
		}
		inline float get_max_unclip_y(float z, float max_proj_y)//returns max actual y coord that will get successfully projected on near plane without getting clipped
		{
			z = z - view_depth;
			return -1 * (max_proj_y * (z / n_plane));
		}

		//these friend classes are concrete classes which override pure virtual function ( pure(app* th) ) of abstract class "abst"
		//These friend classes have been used for partitioning the code of class "app" without creating too many functions(member or helper) of different names 
		friend class c_prog;
		friend class c_texture;
		friend class c_matrix;
		friend class c_buffer;
		friend class c_live;
		friend class c_model;
		

	};

};