#pragma once

unsigned int ColorConvert16(stColor4);

stVector3 RoundDec3forVector(stVector3);

template <typename U, typename T>
noDel_ptr<U> dynamic_noDel_cast(noDel_ptr<T> ptr) {
    T* _ptr = ptr.get();
    return noDel_ptr<U>(dynamic_cast<U*>(_ptr));
}