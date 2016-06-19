/* 

Amine Rehioui
Created: October 31st 2011

*/

namespace shoot
{
    //! AnimationVisitor class
	public class AnimationVisitor : Visitor
	{		
        //! playback types
		public enum PlaybackType
		{
			Once,
			Toggle,
			Loop			
		};

		//! constructor
		public AnimationVisitor()
        {
        }

        //! destructor
        ~AnimationVisitor()
        {
        }

		//! Reads/Writes struct properties from/to a stream
		public override void Serialize(PropertyStream stream)
        {
            base.Serialize(stream);

            m_PlaybackType = (PlaybackType)stream.Serialize<int>("PlaybackType");
            m_MaxPlayCount = stream.Serialize<int>("MaxPlayCount");
        }

		//! visits a particular entity
        public override void Visit(Entity target)
        {
            m_PlayCount = 0;
            base.Visit(target);
        }

        protected int m_PlayCount = 0;

        // properties
        protected PlaybackType m_PlaybackType = PlaybackType.Once;
        protected int m_MaxPlayCount = -1;
	}
}

