//
//  DeviceResource.h
//  Lutra
//
//  Created by JasonCheng on 2021/1/15.
//

#ifndef DeviceResource_h
#define DeviceResource_h
#include <memory>

namespace Lutra {
    
    class DeviceResource
    {
    public:
        virtual ~DeviceResource() = default;
        
        void Update() { m_usage = m_usage << 1; };
        
        void UpdateUsage() { m_usage |= 1; };
        uint32_t GetUsage() const { return m_usage; }
        
    protected:
        
        uint32_t m_usage = 0;
    };


}

#endif /* DeviceResource_h */
