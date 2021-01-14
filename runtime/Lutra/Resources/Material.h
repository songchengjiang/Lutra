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
#include "Resource.h"

namespace Lutra {

    class Pass;
    class Material : public Resource
    {
    public:
        ~Material() = default;
        
        virtual void Serialize(WriteStream& stream) override;
        virtual void Deserialize(ReadStream& stream) override;
        
        void AddPass(const std::shared_ptr<Pass>& pass);
        
        std::shared_ptr<Pass>& GetPass(size_t idx) {
            LT_CORE_ASSERT(idx < m_passList.size(), "Out Of Range!");
            return m_passList[idx];
        }
        
        const std::shared_ptr<Pass>& GetPass(size_t idx) const {
            LT_CORE_ASSERT(idx < m_passList.size(), "Out Of Range!");
            return m_passList[idx];
        }
        
        size_t GetPassCount() const { return m_passList.size(); }
        
        const std::vector<std::shared_ptr<Pass>>& GetPasses() const { return m_passList; }
        
    private:
        
        Material();
        friend class ResourceManager;
        
    private:
        
        std::vector<std::shared_ptr<Pass>> m_passList;
    };

}

#endif /* Material_hpp */
