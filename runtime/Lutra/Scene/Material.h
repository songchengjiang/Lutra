//
//  Material.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/4.
//

#ifndef Material_hpp
#define Material_hpp
#include "LutraAssert.h"
#include <vector>

namespace Lutra {

    class Pass;
    class Material
    {
    public:
        Material() = default;
        ~Material() = default;
        
        void AddPass(const Pass& pass);
        
        Pass& GetPass(uint32_t idx) {
            LT_CORE_ASSERT(idx < m_passList.size(), "Out Of Range!");
            return m_passList[idx];
        }
        
        const Pass& GetPass(uint32_t idx) const {
            LT_CORE_ASSERT(idx < m_passList.size(), "Out Of Range!");
            return m_passList[idx];
        }
        
        size_t GetPassCount() const { return m_passList.size(); }
        
    private:
        
        std::vector<Pass> m_passList;
    };

}

#endif /* Material_hpp */
