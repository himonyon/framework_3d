#pragma once

unsigned int ColorConvert16(stColor4);

stVector3 RoundDec3forVector(stVector3);
float RoundDec3forFloat(float);


_NODISCARD static inline auto ToXMVECTOR(const DirectX::XMFLOAT3& vec)
{
    return DirectX::XMLoadFloat3(&vec);
}

_NODISCARD static inline auto ToXMFLOAT3(const DirectX::XMVECTOR& vec)
{
    DirectX::XMFLOAT3 rv;

    DirectX::XMStoreFloat3(&rv, vec);

    return rv;
}

template <typename U, typename T>
noDel_ptr<U> dynamic_noDel_cast(noDel_ptr<T> ptr) {
    T* _ptr = ptr.get();
    return noDel_ptr<U>(dynamic_cast<U*>(_ptr));
}

template <typename U, typename T>
noDel_ptr<U> static_noDel_cast(noDel_ptr<T> ptr) {
    T* _ptr = ptr.get();
    return noDel_ptr<U>(static_cast<U*>(_ptr));
}

//•¶š—ñ‚ğ‹æØ‚é
void Split(char split_char, char* buffer, std::vector<std::string>& out);
//•¶š—ñ‚ğ’u‚«Š·‚¦‚é
void Replace(char search_char, char replace_char, char* buffer);