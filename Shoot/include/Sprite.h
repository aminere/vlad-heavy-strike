/* 

Amine Rehioui
Created: April 17th 2010

*/

#ifndef _SPRITE_H_INCLUDED_
#define _SPRITE_H_INCLUDED_

#include "Image.h"

#include "Texture.h"

namespace shoot
{
	//! Sprite entity
	/** Contains common 2D animation and transformation features needed by gameplay */
	class Sprite : public Image				 
	{
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(Sprite, Image);

		//! Constructor
		Sprite();

		//! Destructor
		virtual ~Sprite();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();		

		//! Reloads the sprite given a sprite resource path
		void SetPath(std::string strPath);

		//! Returns the number of animations
		u32 GetNumAnimations() const { return m_aAnimations.GetSize(); }

		//! returns an animation name
		/** \param animIndex: specifies the animation index. Default is -1 to get the current animation name. */
		std::string GetAnimationName(s32 animIndex = -1);

		//! Returns the current animation index
		u32 GetCurrentAnimationIndex() const { return m_CurrentAnimation; }

		//! sets the current animation from an animation name
		/** \param strName: name of the animation
			\param frameIndex: specifies the starting frame */
		void SetCurrentAnimation(const char* strName, u32 frameIndex = 0);

		//! Sets the current animation from an animation index
		/** \param animIndex: index of the animation
			\param frameIndex: specifies the starting frame */
		void SetCurrentAnimation(u32 animIndex, u32 frameIndex = 0);

		//! Returns the number of frames
		u32 GetNumFrames() const { return m_aAnimations[m_CurrentAnimation].aFrames.size(); }

		//! returns the current frame
		u32 GetCurrentFrame() const { return m_CurrentFrame; }

		//! Sets the current frame
		void SetCurrentFrame(u32 index);

		//! Start playback at the specified frame
		/** \param bLoop: specifies if played in a loop
			\param frameIndex: specifies the starting frame. Default is -1 to continue at the current frame. */
		void Play(bool bLoop = false, s32 frameIndex = -1);

		//! stops the playback
		void Stop();

		//! returns the playback status
		bool IsStopped() const { return m_bIsStopped; }

		//! Get the bounding box
		AABBox2D GetBoundingBox() const;

		//! Structure describing an animation
		struct Animation : public ISerializableStruct
		{
			//! constructor
			Animation() 
			: framesPerSecond(30) // it's better to expose the FPS in the xml
			, fFrameDeltaTime(0.03f)
			{
			}

			std::vector< Reference<Texture> > aFrames;
			f32 fFrameDeltaTime;

			// properties
			std::string name;
			s32 framesPerSecond;
			Array<std::string> aFramePaths;			

			//! Initializes the Animation frames
			void Init();

			//! serialize the animation
			void Serialize(PropertyStream& stream)
			{
				stream.Serialize(PT_String, "Name", &name);
				stream.Serialize(PT_Int, "FPS", &framesPerSecond);
				stream.SerializeArray("Frames", &aFramePaths, PT_File);				
			}
		};

		private:

		//! Deletes the animations
		void DeleteAnimations();

		Array<Animation> m_aAnimations;
		
		u32 m_CurrentFrame;
		f32 m_fCurrentFrameDuration;

		bool m_bIsStopped;
		bool m_bIsLooping;

		// properties
		std::string m_strPath;
		s32 m_CurrentAnimation;

		// used to expose the animations as an enumeration in the editor
		static const u32 AnimationLiteralSize = 64;
		char **m_ppAnimationLiterals; 
	};
}

#endif // _SPRITE_H_INCLUDED_