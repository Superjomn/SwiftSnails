#pragma once
#include "common.h"

namespace swift_snails {

class Vec {
public:
    typedef double value_type;

    Vec() {
    }

    ~Vec() {
        if(_data != NULL) delete _data;
    }

    Vec(size_t size) {
        init(size);
    }

    Vec(const Vec& other) {
        if(_size != other.size()) {
            if(_data) {
                delete _data;
                _data = NULL;
                _size = 0;
            }
            init(other.size());
        }
        for(size_t i = 0; i < size(); i++) {
            data()[i] = other[i];
        }
    }

    Vec(Vec&& other) {
        _data = other._data;
        _size = other._size;
        other._data = NULL;
        other._size = 0;
    }

    Vec& operator= (const Vec& other) {
        if(this != &other) {
            _size = other.size(); 
            reset(_size);
            for(size_t i = 0; i < _size; i++) {
                data()[i] = other[i];
            }
        }
        return *this;
    }

    friend std::vector<Vec> outer(const Vec &a, const Vec &b) {
        CHECK_GT(a.size(), 0);
        CHECK_GT(b.size(), 0);

        std::vector<Vec> vs;
        for(size_t i = 0; i < a.size(); i ++) {
            Vec v(b.size());
            vs.push_back(std::move(b));
        }
        for(size_t i = 0; i < a.size(); i++) {
            auto &v = vs[i];
            for(size_t j = 0; j < b.size(); j++) {
                v[j] = a[i] * b[j];
            }
        }
        return std::move(vs);
    }

    void init(size_t size, bool random_init = false) {
        CHECK_GT(size, 0);
        CHECK(! _data) << "data can be inited only once";
        //_data.reset(new value_type[size]);
        reset(size);
        for(size_t i = 0; i < _size; ++i) {
            data()[i] = 0.0;
        }
        if(random_init) randInit(0.0);
    }

    void random() {
        randInit(0.0);
    }

	value_type& operator[](size_t i) {
        CHECK_GE(i, 0);
        CHECK_LE(i, size());
		return _data[i];
	}
	const value_type& operator[](size_t i) const {
        CHECK(i >= 0 && i < size());
		return _data[i];
	}

    value_type dot(const Vec &other) const {
        CHECK_EQ(size(), other.size());
        value_type res = 0.0;
        for(size_t i = 0; i < size(); i ++) {
            res += data()[i] * other[i];
        }
        return std::move(res);
    }

    friend std::ostream & operator<< (std::ostream &os, const Vec &other) {
        os << "Vec:\t";
        for (uint32_t i = 0; i < other.size(); ++i) {
            os << other[i] << " ";
        }
	    return os;
    }

    std::string to_str() const {
        std::stringstream ss;
        ss << "Vec:\t";
        for (uint32_t i = 0; i < size(); ++i) {
            ss << data()[i] << " ";
        }
        return std::move(ss.str());
    }

    size_t size() const {
        return _size;
    }

    value_type* data() {
        return _data;
    }
    const value_type* data() const {
        return _data;
    }

    friend Vec operator*(const Vec &vec, value_type b) {
        Vec v(vec);
        for(size_t i = 0; i < v.size(); i ++) {
            v[i] *= b;
        }
        return std::move(v);
    }
    friend Vec operator*(value_type b, const Vec &vec) {
        return std::move(vec * b);
    }
    friend Vec operator*(const Vec &a, const Vec &b) {
        CHECK_EQ(a.size(), b.size());
        Vec tmp(a);
        for(size_t i = 0; i < a.size(); i++) {
            tmp[i] *= b[i];
        }
        return std::move(tmp);
    }
    friend Vec operator/(const Vec &vec, value_type b) {
        Vec v(vec);
        for(size_t i = 0; i < v.size(); i ++) {
            v[i] /= b;
        }
        return std::move(v);
    }
    friend Vec operator/(value_type b, const Vec &vec) {
        return std::move(1.0/b  * vec);
    }
    friend Vec operator/(const Vec &a, const Vec &b) {
        Vec v(a);
        for(size_t i = 0; i < v.size(); i ++) {
            v[i] /= b[i];
        }
        return std::move(v);
    }
    friend Vec operator+(const Vec &vec, value_type b) {
        Vec v(vec);
        for(size_t i = 0; i < v.size(); i ++) {
            v[i] += b;
        }
        return std::move(v);
    }
    friend Vec operator+(value_type b, const Vec &vec) {
        return std::move(vec + b);
    }
    friend Vec operator-(const Vec &vec, value_type b) {
        Vec v(vec);
        for(size_t i = 0; i < v.size(); i ++) {
            v[i] -= b;
        }
        return std::move(v);
    }
    friend Vec operator-(value_type b, const Vec &vec) {
        return std::move( -1.0 * vec + b);
    }
    friend Vec operator-(const Vec &a, const Vec &b) {
        Vec v(a);
        for(size_t i = 0; i < v.size(); i ++) {
            v[i] -= b[i];
        }
        return std::move(v);
    }
    friend Vec operator+= (Vec &a, const Vec &b) {
        CHECK_EQ(a.size(), b.size());
		for(size_t i=0; i<a.size(); ++i) {
			a[i] += b[i];
		}
		return a;
    }
    friend Vec operator+= (Vec &a, value_type b) {
		for(size_t i=0; i<a.size(); ++i) {
			a[i] += b;
		}
		return a;
    }
	friend Vec& operator-=(Vec &a, const Vec &b) {
		for(size_t i=0; i<a.size(); ++i) {
			a[i] -= b[i];
		}
		return a;
	}
	friend Vec& operator-=(Vec &a, value_type b) {
		for(size_t i=0; i<a.size(); ++i) {
			a[i] -= b;
		}
		return a;
	}
	friend Vec& operator/=(Vec &a, value_type b) {
		for(size_t i=0; i<a.size(); ++i) {
			a[i] /= b;
		}
		return a;
	}


protected:
	void randInit(float offset=0.5) {
        for(size_t i = 0; i < size(); i++) 
            _data[i] = (rand() / (float) RAND_MAX - 0.5) / _size;
	}

    void reset(size_t size_) {
        CHECK_GT(size_ , 0);
        if (size_ == _size) return;
        if(_data != NULL) {
            delete _data;
            _size = 0;
        }
        _size = size_;
        _data = new value_type[size_];
    }

private:
    //std::unique_ptr<value_type[]> _data;
    value_type *_data = NULL;
    size_t _size{0};
};  // class Vec


Vec sqrt(const Vec &vec) {
    Vec tmp(vec);
    for(size_t i = 0; i < vec.size(); i++) {
        tmp[i] = std::sqrt(tmp[i]);
    }
    return std::move(tmp);
}



};  // end namespace swift_snails

