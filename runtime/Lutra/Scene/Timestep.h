//
//  Timestep.h
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#ifndef Timestep_h
#define Timestep_h

namespace Lutra {

    class Timestep
    {
    public:
        Timestep(float time = 0.0f)
            : m_time(time)
        {
        }

        operator float() const { return m_time; }

        float GetSeconds() const { return m_time; }
        float GetMilliseconds() const { return m_time * 1000.0f; }
        
    private:
        float m_time;
    };

}

#endif /* Timestep_h */
