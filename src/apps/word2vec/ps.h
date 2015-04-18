#include "../../utils/all.h"
using namespace swift_snails;

struct Word2VecGrad {

    Word2VecGrad() {
        if(_len_vec == 0) 
            _len_vec = global_config().get_config("len_vec").to_int32();
        init();
    }

    Word2VecGrad(const Word2VecGrad& other) {
        init();
        h_grad() = other.h_grad();
        v_grad() = other.v_grad();
        n_h() = other.n_h();
        n_v() = other.n_v();
    }

    void init() {
        CHECK(_len_vec > 0);
        if(h_grad().size() == 0) {
            //RAW_DLOG(INFO, ">  h reset new memory");
            h_grad().init(_len_vec);
            v_grad().init(_len_vec);
        }
    }

    Word2VecGrad& operator= (const Word2VecGrad& other) {
        init();
        h_grad() = other.h_grad();
        v_grad() = other.v_grad();
        n_h() = other.n_h();
        n_v() = other.n_v();
    }

    void merge_with(const Word2VecGrad &other) {
        h_grad() += other.h_grad();
        v_grad() += other.v_grad();
        n_v() += other.n_v();
        n_h() += other.n_h();
    }

    void accu_v(Vec& grad) {
        v_grad() += grad;
        _n_v ++;
    }

    void accu_h(Vec& grad) {
        h_grad() += grad;
        _n_h ++;
    }

    void norm() {
        if(_n_h > 0) {
            h_grad() /=  static_cast<double>(_n_h);
        }
        if(_n_v > 0) {
            v_grad() /= static_cast<double>(_n_v);
        }
        _n_h = 0;
        _n_v = 0;
    }
    /*
    Word2VecGrad normed() const {
        Word2VecGrad val(_len_vec);
        if(_n_h > 0) {
            val.h_grad() /= _n_h;
        }
        if(_n_v > 0) {
            val.v_grad() /= _n_v;
        }
        return std::move(val);
    }
    */

    void reset() {
        _n_v = 0;
        _n_h = 0;
        for(int i = 0; i < len_vec(); i++) {
            v_grad()[i] = 0.0;
            h_grad()[i] = 0.0;
        }
    }

    const Vec& h_grad() const {
        return _h;
    }
    const Vec& v_grad() const {
        return _v;
    }
    Vec& h_grad() {
        return _h;
    }
    Vec& v_grad() {
        return _v;
    }
    int& n_v() {
        return _n_v;
    }
    int& n_h() {
        return _n_h;
    }
    int n_h() const {
        return _n_h;
    }
    int n_v() const {
        return _n_v;
    }
    int len_vec() const {
        return _len_vec;
    }
    friend std::ostream& operator<< (std::ostream& os, const Word2VecGrad& grad) {
        os << "nh\t" << grad.n_h() << "\tnv\t" << grad.n_v() << "\t";
        os << "h:\t";
        for(int i = 0; i < grad.len_vec(); i++) {
            os << grad.h_grad()[i] << " ";
        }
        os << "\t";
        os << "v:\t";
        for(int i = 0; i < grad.len_vec(); i++) {
            os << grad.v_grad()[i] << " ";
        }
        return os;
    }


private:
    static int _len_vec;
    int _n_v{0};
    int _n_h{0};
    Vec _h;
    Vec _v;
};  // class Word2VecGrad



/*
 * parameter structure
 */
struct Word2VecParam {
public:
    Word2VecParam() {
        if(_len_vec == 0) 
            _len_vec = global_config().get_config("len_vec").to_int32();
        init();
    }

    Word2VecParam(const Word2VecParam& other) {
        init();
        h() = other.h();
        v() = other.v();
        h2sum() = other.h2sum();
        v2sum() = other.v2sum();
    }

    void random_init() {
        h().random();
        v().random();
    }

    void init(bool rand_init = false) {
        CHECK(_len_vec > 0);
        if(_h.size() == 0) {
            h().init(_len_vec);
            v().init(_len_vec);
            h2sum().init(_len_vec);
            v2sum().init(_len_vec);
        }
        if(rand_init) {
            std::random_device rd;
            std::normal_distribution<> d(0,1);
            for(int i = 0; i < _len_vec; i++) {
                h()[i] = d(rd);
                v()[i] = d(rd);
            }
        } 
    }
    /*
     * copy value without copy grad
     */
    void copy_value(const Word2VecParam& other) {
        h() = other.h();
        v() = other.v();
    }

    Word2VecParam& operator= (const Word2VecParam &other) {
        init();
        h() = other.h();
        v() = other.v();
        h2sum() = other.h2sum();
        v2sum() = other.v2sum();
        return *this;
    }

    const Vec& h() const {
        return _h;
    }
    const Vec& v() const {
        return _v;
    }
    const Vec& h2sum() const {
        return _h2sum;
    }
    const Vec& v2sum() const {
        return _v2sum;
    }
    Vec& h() {
        return _h;
    }
    Vec& v() {
        return _v;
    }
    Vec& h2sum() {
        return _h2sum;
    }
    Vec& v2sum() {
        return _v2sum;
    }
    int len_vec() const {
        return _len_vec;
    }

    friend std::ostream& operator<< (std::ostream &os, const Word2VecParam &param) {
        os << "h:\t";
        for(int i = 0; i < param.len_vec(); i ++) {
            os << param.h()[i] << " ";
        }
        os << "v:\t";
        for(int i = 0; i < param.len_vec(); i ++) {
            os << param.v()[i] << " ";
        }
        return os;
    }

private:
    Vec _h;
    Vec _v;
    Vec _h2sum;
    Vec _v2sum;
    static int _len_vec;
};  // class Word2VecParam


int Word2VecGrad::_len_vec = 0;
int Word2VecParam::_len_vec = 0;




