#pragma once

/// <summary>
/// 値には参照したいがdeleteしてはいけないポインタクラス
/// 他の変数がポインタの所有権を持っていることを明示的に表す
/// </summary>
template <class T>
class noDel_ptr {
public:
    //演算子オーバーロード
    noDel_ptr<T>& operator =(const noDel_ptr<T>& src)
    {
        // 引数で受け取ったインスタンス（参照）の内容で上書きする
        this->ptr = src.ptr;
        // 自分自身の参照を返す
        return *this;
    }
    noDel_ptr<T>& operator =(const std::nullptr_t null)
    {
        this->ptr = nullptr;
        return *this;
    }
    bool operator !=(const noDel_ptr<T> src)
    {
        if (ptr != src.ptr) return true;
        return false;
    }
    bool operator ==(const noDel_ptr<T> src)
    {
        if (ptr == src.ptr) return true;
        return false;
    }
    bool operator !=(const std::nullptr_t null)
    {
        if(ptr != null) return true;
        return false;
    }
    bool operator ==(const std::nullptr_t null)
    {
        if (ptr == null) return true;
        return false;
    }
    // 保持しているポインタを透過的に扱えるようにする
    T* operator->() {
        return ptr;
    }


    //コンストラクタ
    noDel_ptr() {};
    template<class U>
    explicit noDel_ptr(const noDel_ptr<U>& src) {
        ptr = src.ptr;
    }
    noDel_ptr(std::nullptr_t) { ptr = nullptr; }
    explicit noDel_ptr(T* ptr) {
        //アドレスを保持する
        this->ptr = ptr;
    }

    ~noDel_ptr() {}


    //生ポインタの取得
    T* get() {
        return ptr;
    }

private:
    // 外部からアクセスできない形でポインタ変数を宣言
    T* ptr = nullptr;
};