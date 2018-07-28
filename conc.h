//header for declaration of concrete classes

namespace N_app
{
	//after declaring a concrete class here make it friend of app class in app.h

	class c_prog : public abst
	{
		GLuint vert_shad;
		GLuint cntrl_shad;
		GLuint eval_shad;
		GLuint geom_shad;
		GLuint frag_shad;
	public:
		void pure(app* th);

	};
	

	class c_buffer :public abst
	{
	public:

		void pure(app* th);

	};

	
	class c_texture :public abst
	{
	public:
		void pure(app* th);

	};

	class c_matrix :public abst
	{

	public:
		void pure(app* th);


	};

	class c_model :public abst
	{
	public:
		void pure(app* th);

	};

	class c_live :public abst
	{

	public:
		void pure(app* th);


	};


};
