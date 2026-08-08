#include "pch.h"
#include "Visuals/Types.hpp"

class Helpers
{
public:
    static const std::string& GetLowerStructName(SDK::UStruct* structure)
    {
        static std::unordered_map<SDK::UStruct*, std::string> cache;

        auto it = cache.find(structure);
        if (it != cache.end())
            return it->second;

        auto [insertedIt, inserted] = cache.emplace(structure, ToLowerCopy(structure->Name.ToString()));
        return insertedIt->second;
    }

    static std::string ToLowerCopy(std::string value)
    {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c)
        { 
            return static_cast<char>(std::tolower(c)); 
        });
        return value;
    }

    template <typename Type, typename LookupType, size_t LookupCount>
    static Type ResolveType(std::unordered_map<SDK::UClass*, Type>& cache, SDK::AActor* actor, const LookupType (&lookups)[LookupCount])
    {
        if (!actor || !actor->Class)
            return Type::None;

        auto it = cache.find(actor->Class);
        if (it != cache.end())
            return it->second;

        Type type = Type::None;

        for (SDK::UStruct* pStruct = actor->Class; pStruct; pStruct = static_cast<SDK::UStruct*>(pStruct->SuperStruct))
        {
            const std::string& className = GetLowerStructName(pStruct);

            for (const auto& entry : lookups)
            {
                if (className.find(entry.Keyword) != std::string::npos)
                {
                    type = entry.Type;
                    break;
                }
            }

            if (type != Type::None)
                break;
        }

        cache.emplace(actor->Class, type);
        return type;
    }

    static Types::EnemyType ResolveEnemyType(std::unordered_map<SDK::UClass*, Types::EnemyType>& cache, SDK::AActor* actor)
    {
        return ResolveType(cache, actor, Types::g_EnemyLookup);
    }

    static Types::ItemType ResolveItemType(std::unordered_map<SDK::UClass*, Types::ItemType>& cache, SDK::AActor* actor)
    {
        return ResolveType(cache, actor, Types::g_ItemLookup);
    }
};