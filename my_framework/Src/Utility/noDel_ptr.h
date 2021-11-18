#pragma once

/// <summary>
/// �l�ɂ͎Q�Ƃ�������delete���Ă͂����Ȃ��|�C���^�N���X
/// ���̕ϐ����|�C���^�̏��L���������Ă��邱�Ƃ𖾎��I�ɕ\��
/// </summary>
template <class T>
class noDel_ptr {
    template<class> friend class noDel_ptr;
public:
    //���Z�q�I�[�o�[���[�h
    noDel_ptr<T>& operator =(const noDel_ptr<T>& src)
    {
        // �����Ŏ󂯎�����C���X�^���X�i�Q�Ɓj�̓��e�ŏ㏑������
        this->ptr = src.ptr;
        // �������g�̎Q�Ƃ�Ԃ�
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
    // �ێ����Ă���|�C���^�𓧉ߓI�Ɉ�����悤�ɂ���
    T* operator->() {
        return ptr;
    }


    //�R���X�g���N�^
    noDel_ptr() {};
    template<class U>
    noDel_ptr(const noDel_ptr<U>& src) {
        ptr = src.ptr;
    }
    noDel_ptr(std::nullptr_t) { ptr = nullptr; }
    explicit noDel_ptr(T* ptr) {
        //�A�h���X��ێ�����
        this->ptr = ptr;
    }

    ~noDel_ptr() {}

private:
    // �O������A�N�Z�X�ł��Ȃ��`�Ń|�C���^�ϐ���錾
    T* ptr = nullptr;
};