using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;

namespace ShootLib
{
    public class XNADriverBridge
    {
        //! transformation states
        public enum TransformState
        {
            Projection,
            View,
            World,
            Count
        };

        //! render states
        public enum RenderState
        {
            DepthTesting,
            DepthWriting,
            AlphaBlending,
            _2DTextureMapping,
            DepthFunc, // Uses E_ComparisonFunction enumeration
            SrcBlend, // Uses E_BlendFactor enumeration
            DestBlend, // Uses E_BlendFactor enumeration
            PointSize,
            PointSprite
        };

        //! Texture info
        public struct TextureInfo
        {
            public int Index;
            public int Width;
            public int Height;
        };

        public static void Begin(ClearOptions options, Color color, float depth, int stencil)
        {
            Device.Clear(options, color, depth, stencil);
            MainEffect.Begin();
		    MainEffect.CurrentTechnique.Passes[0].Begin();
        }

        public static void End()
        {
            MainEffect.CurrentTechnique.Passes[0].End();
		    MainEffect.End();
        }

        public static void Draw2DQuad(Vector2 vSize, bool bHFlip /*= false*/, bool bVFlip /*= false*/)
        {
            Vector2 uvStart = new Vector2(bHFlip ? 1.0f : 0.0f, bVFlip ? 1.0f : 0.0f);	
            Vector2 uvEnd = new Vector2(1.0f, 1.0f) - uvStart;            

            PositionTextureVertices[0].Position = new Vector3(0.0f, 0.0f, 0.0f);
            PositionTextureVertices[0].TextureCoordinate = new Vector2(uvStart.X, uvStart.Y);

            PositionTextureVertices[1].Position = new Vector3(vSize.X, 0.0f, 0.0f);
            PositionTextureVertices[1].TextureCoordinate = new Vector2(uvEnd.X, uvStart.Y);

            PositionTextureVertices[2].Position = new Vector3(0.0f, vSize.Y, 0.0f);
            PositionTextureVertices[2].TextureCoordinate = new Vector2(uvStart.X, uvEnd.Y);

            PositionTextureVertices[3].Position = new Vector3(0.0f, vSize.Y, 0.0f);
            PositionTextureVertices[3].TextureCoordinate = new Vector2(uvStart.X, uvEnd.Y);

            PositionTextureVertices[4].Position = new Vector3(vSize.X, 0.0f, 0.0f);
            PositionTextureVertices[4].TextureCoordinate = new Vector2(uvEnd.X, uvStart.Y);

            PositionTextureVertices[5].Position = new Vector3(vSize.X, vSize.Y, 0.0f);
            PositionTextureVertices[5].TextureCoordinate = new Vector2(uvEnd.X, uvEnd.Y);	

            Device.VertexDeclaration = PositionTextureVertexDeclaration;
            Device.DrawUserPrimitives<VertexPositionTexture>(PrimitiveType.TriangleList, PositionTextureVertices, 0, 2);
        }
                
        public static void SetViewPort(Viewport viewport)
        {
            Device.Viewport = viewport;
        }

        public static void SetTransform(TransformState state, Matrix matrix)
        {
            switch (state)
            {
                case TransformState.Projection: MainEffect.Projection = matrix; break;
                case TransformState.View: MainEffect.View = matrix; break;
                case TransformState.World: MainEffect.World = matrix; break;
            }
        }

        public static void SetRenderState(RenderState state, bool bEnable)
        {
		    switch(state)
		    {
		    case RenderState.DepthTesting: Device.RenderState.DepthBufferEnable = bEnable; break;
		    case RenderState.DepthWriting: Device.RenderState.DepthBufferWriteEnable = bEnable;	break;
		    case RenderState.AlphaBlending: Device.RenderState.AlphaBlendEnable = bEnable; break;
		    case RenderState._2DTextureMapping: MainEffect.TextureEnabled = bEnable; break;
		    }
        }

        public static void SetRenderState(RenderState state, int iValue)
        {
            CompareFunction[] xnaCompFuncs = { CompareFunction.Never, 
            								   CompareFunction.Less, 
            								   CompareFunction.Equal, 
            								   CompareFunction.LessEqual, 
            								   CompareFunction.Greater, 
            								   CompareFunction.NotEqual, 
            								   CompareFunction.GreaterEqual, 
            								   CompareFunction.Always };		

            Blend[] xnaBlendFactors = { Blend.Zero,
            							Blend.One, 
            							Blend.SourceColor, 
            							Blend.InverseSourceColor, 
            							Blend.SourceAlpha, 
            							Blend.InverseSourceAlpha,
            							Blend.DestinationAlpha, 
            							Blend.InverseDestinationAlpha, 
            							Blend.DestinationColor, 
            							Blend.InverseDestinationColor, 
            							Blend.SourceAlphaSaturation };

            switch (state)
            {
                case RenderState.DepthFunc: Device.RenderState.DepthBufferFunction = xnaCompFuncs[iValue]; break;
                case RenderState.SrcBlend: Device.RenderState.AlphaSourceBlend = xnaBlendFactors[iValue]; break;
                case RenderState.DestBlend: Device.RenderState.AlphaDestinationBlend = xnaBlendFactors[iValue]; break;
            }
        }

        public static void SetTexture(int index)
        {
            if (index >= 0)
            {
                MainEffect.TextureEnabled = true;
                Texture2D texture = TextureList[index] as Texture2D;
			    MainEffect.Texture = texture;
            }
            else
            {
                MainEffect.TextureEnabled = false;
            }
        }

        public static void SetMaterialColor(Vector3 color)
        {
            MainEffect.DiffuseColor = color;
        }

        public static void Clear(ClearOptions options, Color color, float depth, int stencil)
        {
            Device.Clear(options, color, 0, 0);
        }

        public static void SetReferences(GraphicsDeviceManager deviceManager, ContentManager contentManager)
        {
		    DeviceManager = deviceManager;
		    Content = contentManager;
		    GraphicsDevice currentDevice = DeviceManager.GraphicsDevice;				

		    bool bDeviceChanged = (currentDevice != Device);

		    // create device dependent objects		
		    if(MainEffect == null || bDeviceChanged)
		    {				
			    MainEffect = new BasicEffect(currentDevice, null);			
		    }		

		    if(PositionTextureVertexDeclaration == null || bDeviceChanged)
		    {
			    PositionTextureVertexDeclaration = new VertexDeclaration(currentDevice, VertexPositionTexture.VertexElements);
		    }		

		    Device = currentDevice;
        }

        public static void Init()
	    {
		    TextureList = new ArrayList();

		    PositionTextureVertices = new VertexPositionTexture[6];		
	    }

        public static TextureInfo GetTexture(String path)
        {
            int index = -1;
            int currentIndex = 0;
            Texture2D texture = null;
            foreach (object o in TextureList)
            {
                texture = o as Texture2D;
                if (texture.Name == path)
                {                    
                    index = currentIndex;
                    break;
                }
                currentIndex++;
            }

            if (index >= 0)
            {
                TextureInfo info;
                info.Index = index;
                info.Width = texture.Width;
                info.Height = texture.Height;
                return info;
            }
            else
            {
                texture = Content.Load<Texture2D>(path);
                texture.Name = path;
                TextureList.Add(texture);
                TextureInfo info;
                info.Index = TextureList.Count - 1;
                info.Width = texture.Width;
                info.Height = texture.Height;
                return info;
            }            
        }

        static private GraphicsDeviceManager DeviceManager;
        static private ContentManager Content;
        static private GraphicsDevice Device;
        static BasicEffect MainEffect;
        static ArrayList TextureList;

        static VertexPositionTexture[] PositionTextureVertices;
		static VertexDeclaration PositionTextureVertexDeclaration;
    }
}
