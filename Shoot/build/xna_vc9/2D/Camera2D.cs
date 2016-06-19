/* 

Amine Rehioui
Created: September 26th 2010

*/

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace shoot
{
    //! 2D Camera entity
    public class Camera2D : Entity2D
    {
        //! called during the rendering of the entity		
		public override void Render()
        {
            Viewport v = Engine.Instance().Device.Viewport;
            Engine.Instance().MainEffect.Projection = GetProjectionMatrix(v.Width, v.Height);		    

		    // set the view matrix
		    Matrix _2DMatrix = Matrix.CreateScale(new Vector3(GetScale().X, GetScale().Y, 1.0f));
            _2DMatrix.Translation = new Vector3(GetPosition().X, GetPosition().Y, 0.0f);		    
		    Engine.Instance().MainEffect.View = _2DMatrix;		    
        }

		//! returns the projection matrix
		/** TODO: only compute if necessary */
		Matrix GetProjectionMatrix(int screenWidth, int screenHeight)
        {            
            return Matrix.CreateOrthographicOffCenter(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
        }
    }
}
