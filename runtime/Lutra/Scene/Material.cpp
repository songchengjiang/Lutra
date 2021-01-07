//
//  Material.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#include "Material.h"
#include "Pass.h"

namespace Lutra {

    void Material::AddPass(const Pass& pass)
    {
        m_passList.push_back(pass);
    }

}
