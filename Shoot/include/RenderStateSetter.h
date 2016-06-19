/* 

Amine Rehioui
Created: April 5th 2012

*/

#ifndef _RENDER_STATE_SETTER_H_INCLUDED_
#define _RENDER_STATE_SETTER_H_INCLUDED_

namespace shoot
{
	//! RenderStateSetter
	class RenderStateSetter : public Object
	{
		DECLARE_OBJECT(RenderStateSetter, Object);

	public:

		//! begins rendering with this setter
		virtual void Begin() { }

		//! end rendering with this setter
		virtual void End() { }
	};

	//! PointSpriteStateSetter
	class PointSpriteStateSetter : public RenderStateSetter
	{
	public:

		//! constructor
		PointSpriteStateSetter(f32 fPointSize)
			: m_fPointSize(fPointSize)
		{
		}

		//! begins rendering with this setter
		void Begin();

		//! ends rendering with this setter
		void End();

		//! sets the point size
		void SetPointSize(f32 fSize) { m_fPointSize = fSize; }

	private:

		f32 m_fPointSize;
	};

	//! SkyboxStateSetter
	class SkyBoxStateSetter : public RenderStateSetter
	{
	public:

		//! begins rendering with this setter
		void Begin();

		//! end rendering with this setter
		void End();
	};
}

#endif // _RENDER_STATE_SETTER_H_INCLUDED_

