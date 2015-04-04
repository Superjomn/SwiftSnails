#include <iostream>
/*
 * adagrad support
 */
struct Word2VecGrad {
    explicit Word2VecGrad (int len_vec) :
        len_vec(len_vec)
    { 
        init(len_vec);
    }

    explicit Word2VecGrad {
    }
    void init(int len_vec) {
        this->len_vec = len_vec;
        h.reset(new Vec::zero(len_vec));
        v.reset(new Vec::zero(len_vec));
    }

    Word2VecGrad& operator= (const Word2VecGrad& other) {
        *_h = other.h_grad();
        *_v = other.v_grad();
    }

    void merge_with(const Word2VecGrad &other) {
        *_h += other.h_grad();
        *_v += otehr.v_grad();
        n += other.n();
    }

    void norm() const {
        if(_n > 0) {
            h_grad() = h_grad() / _n;
            v_grad() = v_grad() / _n;
        }
        _n = 0;
    }

    Word2VecGrad normed() const {
        Word2VecGrad val(len_vec);
        if(_n > 0) {
            val.h_grad() = h_grad() / _n;
            val.v_grad() = v_grad() / _n;
        }
        return std::move(val);
    }

    void reset() {
        _n = 0.0;
        for(auto& v : h_grad()) {
            v = 0.0;
        }
        for(auto& v : v_grad()) {
            v = 0.0;
        }
    }

    const Vec& h_grad() const {
        return *_h;
    }
    const Vec& v_grad() const {
        return *_v;
    }
    Vec& h_grad() {
        return *_h;
    }
    Vec& v_grad() {
        return *_v;
    }
    int n() const {
        return _n;
    }
    int len_vec() {
        return len_vec;
    }


private:
    int len_vec{0};
    int _n{0};
    std::unique_ptr<Vec> _h;
    std::unique_ptr<Vec> _v;
};  // class Word2VecGrad


/*
 * parameter structure
 */
struct Word2VecParam {
public:
    explicit Word2VecParam() {
    }

    void init(int len_vec, bool rand_init = false) {
        this->len_vec = len_vec;
        if(rand_init) {
            this->_h.reset(new Vec::random(len_vec));
            this->_v.reset(new Vec::random(len_vec));
        } else {
            this->_h.reset(new Vec::Zero(len_vec));
            this->_v.reset(new Vec::Zero(len_vec));
        }

        grad.init(len_vec);
    }
    /*
     * copy value without copy grad
     */
    void copy_value(const Word2VecParam& other) {
        _h = other.h();
        _v = other.v();
    }

    Word2VecParam& operator= (const Word2VecGrad &other) {
        h() = other.h();
        v() = other.v();
        h2sum() = other.h2sum();
        v2sum() = other.v2sum();
    }

    const Vec& h() const {
        return *_h;
    }
    const Vec& v() const {
        return *_v;
    }
    const Vec& h2sum() const {
        return *_h2sum;
    }
    const Vec& v2sum() const {
        return *_v2sum;
    }
    Vec& h() {
        return *_h;
    }
    Vec& v() {
        return *_v;
    }
    Vec& h2sum() {
        return *_h2sum;
    }
    Vec& v2sum() {
        return *_v2sum;
    }
    int len_vec() {
        return len_vec;
    }

private:
    std::unique_ptr<Vec> _h;
    std::unique_ptr<Vec> _v;
    std::unique_ptr<Vec> _h2sum;
    std::unique_ptr<Vec> _v2sum;
    int len_vec = len_vec;
};  // class Word2VecParam

