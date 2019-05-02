#pragma once

#ifdef _MSC_VER
#  include <intrin.h>
#else
#  include <x86intrin.h>
#endif

#pragma intrinsic(__rdtsc)

/// <summary>
	/// ��莞�Ԃ̌o�߂��m�F����N���X
	/// </summary>
class Timer {
private:

	enum class Type {
		nanoseconds,
		microseconds,
		milliseconds,
		seconds,
		minutes,
		hours
	};

	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	long long time;
	Type type;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Timer() = default;
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_time">�ݒ莞��(�i�m�b)</param>
	Timer(const std::chrono::nanoseconds& _time) noexcept { type = Type::nanoseconds; time = _time.count(); }
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_time">�ݒ莞��(�}�C�N���b)</param>
	Timer(const std::chrono::microseconds& _time) noexcept { type = Type::microseconds; time = _time.count(); }
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_time">�ݒ莞��(�~���b)</param>
	Timer(const std::chrono::milliseconds& _time) noexcept { type = Type::milliseconds; time = _time.count(); }
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_time">�ݒ莞��(�b)</param>
	Timer(const std::chrono::seconds& _time) noexcept { type = Type::seconds; time = _time.count(); }
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_time">�ݒ莞��(��)</param>
	Timer(const std::chrono::minutes& _time) noexcept { type = Type::minutes; time = _time.count(); }
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_time">�ݒ莞��(��)</param>
	Timer(const std::chrono::hours& _time) noexcept { type = Type::hours; time = _time.count(); }

	/// <summary>
	/// ���Ԃ�ݒ肷��
	/// </summary>
	/// <param name="_time">�ݒ莞��(�i�m�b)</param>
	void set(const std::chrono::nanoseconds& _time) noexcept { type = Type::nanoseconds; time = _time.count(); }
	/// <summary>
	/// ���Ԃ�ݒ肷��
	/// </summary>
	/// <param name="_time">�ݒ莞��(�}�C�N���b)</param>
	void set(const std::chrono::microseconds& _time) noexcept { type = Type::microseconds; time = _time.count(); }
	/// <summary>
	/// ���Ԃ�ݒ肷��
	/// </summary>
	/// <param name="_time">�ݒ莞��(�~���b)</param>
	void set(const std::chrono::milliseconds& _time) noexcept { type = Type::milliseconds; time = _time.count(); }
	/// <summary>
	/// ���Ԃ�ݒ肷��
	/// </summary>
	/// <param name="_time">�ݒ莞��(�b)</param>
	void set(const std::chrono::seconds& _time) noexcept { type = Type::seconds; time = _time.count(); }
	/// <summary>
	/// ���Ԃ�ݒ肷��
	/// </summary>
	/// <param name="_time">�ݒ莞��(��</param>
	void set(const std::chrono::minutes& _time) noexcept { type = Type::minutes; time = _time.count(); }
	/// <summary>
	/// ���Ԃ�ݒ肷��
	/// </summary>
	/// <param name="_time">�ݒ莞��(��)</param>
	void set(const std::chrono::hours& _time) noexcept { type = Type::hours; time = _time.count(); }

	/// <summary>
	/// �^�C�}�[���J�n������
	/// </summary>
	void start() noexcept { s = std::chrono::high_resolution_clock::now(); }

	/// <summary>
	/// �ݒ莞�Ԍo�߂������𓾂�
	/// </summary>
	/// <returns>�o�߂��Ă���� true, ����ȊO�� false</returns>
	inline const bool check() const noexcept {
		const auto e = std::chrono::high_resolution_clock::now();
		switch (type)
		{
		case Type::nanoseconds: return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count() >= time;
		case Type::microseconds: return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count() >= time;
		case Type::milliseconds: return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count() >= time;
		case Type::seconds: return std::chrono::duration_cast<std::chrono::seconds>(e - s).count() >= time;
		case Type::minutes: return std::chrono::duration_cast<std::chrono::minutes>(e - s).count() >= time;
		case Type::hours: return std::chrono::duration_cast<std::chrono::hours>(e - s).count() >= time;
		default: return true;
		}
	}

	/// <summary>
	/// �ݒ莞�Ԍo�߂������𓾂�
	/// </summary>
	/// <returns>�o�߂��Ă���� true, ����ȊO�� false</returns>
	operator bool() const noexcept { return check(); }

};

/// <summary>
/// ��莞�Ԃ̌o�߂��m�F����N���X
/// </summary>
class MilliSecTimer {
private:

	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	unsigned long long int startCycle = 0;
	long long time = 0;

#ifdef _MSC_VER
	const double CyclePerMilliSec = 3200000.0;
#else
	const double CyclePerMilliSec = 2800000.0;//���u��
#endif // _MSC_VER

	unsigned long long int getCycle() const {
		unsigned int low, high;

#ifdef _MSC_VER
		return (__rdtsc());
#else
		__asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
#endif // _MSC_VER
		return ((unsigned long long int)low) | ((unsigned long long int)high << 32);
	}

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	MilliSecTimer() = default;
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_time">�ݒ莞��(�~���b)</param>
	MilliSecTimer(const std::chrono::milliseconds& _time) noexcept { time = _time.count(); }

	/// <summary>
	/// ���Ԃ�ݒ肷��
	/// </summary>
	/// <param name="_time">�ݒ莞��(�~���b)</param>
	void set(const std::chrono::milliseconds& _time) noexcept { time = _time.count(); }

	/// <summary>
	/// �^�C�}�[���J�n������
	/// </summary>
	void start() noexcept {
		startCycle = getCycle();
	}

	/// <summary>
	/// �ݒ莞�Ԍo�߂������𓾂�
	/// </summary>
	/// <returns>�o�߂��Ă���� true, ����ȊO�� false</returns>
	inline const bool check() const noexcept {
		return (getCycle() - startCycle) / CyclePerMilliSec >= time;
	}

	/// <summary>
	/// �ݒ莞�Ԍo�߂������𓾂�
	/// </summary>
	/// <returns>�o�߂��Ă���� true, ����ȊO�� false</returns>
	operator bool() const noexcept { return check(); }

	/// <summary>
	/// �o�ߎ��Ԃ��擾����(�~���b)
	/// </summary>
	/// <returns>�v������(�~���b)</returns>
	inline const long long interval() const noexcept {
		return static_cast<long long int>((getCycle() - startCycle) / CyclePerMilliSec);
	}

};

/// <summary>
	/// ���Ԍv�����s���N���X
	/// </summary>
class Stopwatch {
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Stopwatch() = default;

	/// <summary>
	/// �v�����J�n����
	/// </summary>
	inline void start() noexcept {
		s = std::chrono::high_resolution_clock::now();
		e = s;
	}
	/// <summary>
	/// �v�����~����
	/// </summary>
	inline void stop() noexcept {
		e = std::chrono::high_resolution_clock::now();
	}

	/// <summary>
	/// �v�����Ԃ��擾����(�i�m�b)
	/// </summary>
	/// <returns>�v������(�i�m�b)</returns>
	inline const long long nanoseconds() const noexcept { return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count(); }
	/// <summary>
	/// �v�����Ԃ��擾����(�}�C�N���b)
	/// </summary>
	/// <returns>�v������(�}�C�N���b)</returns>
	inline const long long microseconds() const noexcept { return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count(); }
	/// <summary>
	/// �v�����Ԃ��擾����(�~���b)
	/// </summary>
	/// <returns>�v������(�~���b)</returns>
	inline const long long millisecond() const noexcept { return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count(); }
	/// <summary>
	/// �v�����Ԃ��擾����(�b)
	/// </summary>
	/// <returns>�v������(�b)</returns>
	inline const long long second() const noexcept { return std::chrono::duration_cast<std::chrono::seconds>(e - s).count(); }
	/// <summary>
	/// �v�����Ԃ��擾����(��)
	/// </summary>
	/// <returns>�v������(��)</returns>
	inline const long long minutes() const noexcept { return std::chrono::duration_cast<std::chrono::minutes>(e - s).count(); }
	/// <summary>
	/// �v�����Ԃ��擾����(��)
	/// </summary>
	/// <returns>�v������(��)</returns>
	inline const long long hours() const noexcept { return std::chrono::duration_cast<std::chrono::hours>(e - s).count(); }

	/// <summary>
	/// �P�ʕt���̌v�����Ԃ̕�����𓾂�(�i�m�b)
	/// </summary>
	/// <returns>�v�����Ԃ̕�����(�i�m�b)</returns>
	inline const std::string toString_ns() const { return std::to_string(nanoseconds()) + "ns"; }
	/// <summary>
	/// �P�ʕt���̌v�����Ԃ̕�����𓾂�(�}�C�N���b)
	/// </summary>
	/// <returns>�v�����Ԃ̕�����(�}�C�N���b)</returns>
	inline const std::string toString_us() const { return std::to_string(microseconds()) + "us"; }
	/// <summary>
	/// �P�ʕt���̌v�����Ԃ̕�����𓾂�(�~���b)
	/// </summary>
	/// <returns>�v�����Ԃ̕�����(�~���b)</returns>
	inline const std::string toString_ms() const { return std::to_string(millisecond()) + "ms"; }
	/// <summary>
	/// �P�ʕt���̌v�����Ԃ̕�����𓾂�(�b)
	/// </summary>
	/// <returns>�v�����Ԃ̕�����(�b)</returns>
	inline const std::string toString_s() const { return std::to_string(second()) + "s"; }
	/// <summary>
	/// �P�ʕt���̌v�����Ԃ̕�����𓾂�(��)
	/// </summary>
	/// <returns>�v�����Ԃ̕�����(��)</returns>
	inline const std::string toString_m() const { return std::to_string(minutes()) + "m"; }
	/// <summary>
	/// �P�ʕt���̌v�����Ԃ̕�����𓾂�(��)
	/// </summary>
	/// <returns>�v�����Ԃ̕�����(��)</returns>
	inline const std::string toString_h() const { return std::to_string(hours()) + "h"; }

private:

	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	std::chrono::time_point<std::chrono::high_resolution_clock> e;

};
