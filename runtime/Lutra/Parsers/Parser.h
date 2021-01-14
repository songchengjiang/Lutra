//
//  Parser.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/11.
//

#ifndef Parser_hpp
#define Parser_hpp

namespace Lutra {

    class WriteStream;
    class ReadStream;
    class Parser
    {
    public:
        Parser() = default;
        virtual ~Parser() = default;
        
        virtual void Serialize(WriteStream& stream) = 0;
        virtual void Deserialize(ReadStream& stream) = 0;
    };
}

#endif /* Parser_hpp */
