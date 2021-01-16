//
//  Resource.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/10.
//

#include "Resource.h"
#include "Stream.h"

namespace Lutra {

    Resource::Resource()
    {
        m_uuid = sole::uuid4();
    }

    Resource::~Resource()
    {
    }

    void Resource::Serialize(WriteStream& stream)
    {
        stream.WriteValue("uuid", m_uuid.str());
        stream.WriteValue("Name", m_name);
    }

    void Resource::Deserialize(ReadStream& stream)
    {
        std::string str;
        stream.ReadValue("uuid", str);
        m_uuid = sole::rebuild(str);
        stream.ReadValue("Name", m_name);
    }

}
