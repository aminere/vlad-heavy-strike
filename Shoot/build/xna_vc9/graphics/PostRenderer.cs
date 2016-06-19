/* 

Amine Rehioui
Created: September 26th 2010

*/

namespace shoot
{
    public class PostRenderer
    {
        //! post renderer type
        public enum E_Type
        {
            T_3D,
            T_2D
        }
    }

    //! post 3D renderer interface, override for custom rendering at the end of the 3D pass
	abstract class Post3DRenderer : PostRenderer
	{	
		//! custom Post 3D rendering
		public abstract void RenderPost3D();		
	};

	//! post 3D renderer interface, override for custom rendering at the end of the 2D pass
	abstract class Post2DRenderer : PostRenderer
	{	
		//! custom Post 2D rendering
		public abstract void RenderPost2D();		
	};
}
