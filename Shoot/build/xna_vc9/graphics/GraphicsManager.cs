/* 

Amine Rehioui
Created: September 27th 2010

*/

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace shoot
{
    //! provides graphics driver services
    public class GraphicsManager : Singleton<GraphicsManager>
    {
        private VertexPositionTexture[] PositionTextureVertices = new VertexPositionTexture[6];
        private VertexDeclaration PositionTextureVertexDeclaration = new VertexDeclaration(Engine.Instance().Device, VertexPositionTexture.VertexElements);

        //! renders a 2D quad
        public void Draw2DQuad(Vector2 vSize, bool bHFlip, bool bVFlip)
        {
            Vector2 uvStart = new Vector2(bHFlip ? 1.0f : 0.0f, bVFlip ? 1.0f : 0.0f);
            Vector2 uvEnd = new Vector2(1.0f, 1.0f) - uvStart;

            PositionTextureVertices[0].Position = new Vector3(0.0f, 0.0f, 0.0f);
            PositionTextureVertices[0].TextureCoordinate = new Vector2(uvStart.X, uvStart.Y);

            PositionTextureVertices[1].Position = new Vector3(vSize.X, 0.0f, 0.0f);
            PositionTextureVertices[1].TextureCoordinate = new Vector2(uvEnd.X, uvStart.Y);

            PositionTextureVertices[2].Position = new Vector3(0.0f, vSize.Y, 0.0f);
            PositionTextureVertices[2].TextureCoordinate = new Vector2(uvStart.X, uvEnd.Y);

            PositionTextureVertices[3].Position = PositionTextureVertices[2].Position;
            PositionTextureVertices[3].TextureCoordinate = PositionTextureVertices[2].TextureCoordinate;

            PositionTextureVertices[4].Position = PositionTextureVertices[1].Position;
            PositionTextureVertices[4].TextureCoordinate = PositionTextureVertices[1].TextureCoordinate;

            PositionTextureVertices[5].Position = new Vector3(vSize.X, vSize.Y, 0.0f);
            PositionTextureVertices[5].TextureCoordinate = new Vector2(uvEnd.X, uvEnd.Y);

            Engine.Instance().Device.VertexDeclaration = PositionTextureVertexDeclaration;
            Engine.Instance().Device.DrawUserPrimitives<VertexPositionTexture>(PrimitiveType.TriangleList, PositionTextureVertices, 0, 2);
        }

        //! renders a 2D quad
        /** \param srcRect: UV area of the current texture to draw 
            \param destRect: defines the vertex positions */
        public void Draw2DQuad(BoundingBox srcRect, BoundingBox destRect)
        {
            PositionTextureVertices[0].Position = new Vector3(destRect.Min.X, destRect.Min.Y, 0.0f);
            PositionTextureVertices[0].TextureCoordinate = new Vector2(srcRect.Min.X, srcRect.Min.Y);

            PositionTextureVertices[1].Position = new Vector3(destRect.Max.X, destRect.Min.Y, 0.0f);
            PositionTextureVertices[1].TextureCoordinate = new Vector2(srcRect.Max.X, srcRect.Min.Y);

            PositionTextureVertices[2].Position = new Vector3(destRect.Min.X, destRect.Max.Y, 0.0f);
            PositionTextureVertices[2].TextureCoordinate = new Vector2(srcRect.Min.X, srcRect.Max.Y);

            PositionTextureVertices[3].Position = PositionTextureVertices[2].Position;
            PositionTextureVertices[3].TextureCoordinate = PositionTextureVertices[2].TextureCoordinate;

            PositionTextureVertices[4].Position = PositionTextureVertices[1].Position;
            PositionTextureVertices[4].TextureCoordinate = PositionTextureVertices[1].TextureCoordinate;

            PositionTextureVertices[5].Position = new Vector3(destRect.Max.X, destRect.Max.Y, 0.0f);
            PositionTextureVertices[5].TextureCoordinate = new Vector2(srcRect.Max.X, srcRect.Max.Y);

            Engine.Instance().Device.VertexDeclaration = PositionTextureVertexDeclaration;
            Engine.Instance().Device.DrawUserPrimitives<VertexPositionTexture>(PrimitiveType.TriangleList, PositionTextureVertices, 0, 2);
        }
    }
}
