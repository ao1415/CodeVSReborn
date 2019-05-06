#pragma once

#ifdef _MSC_VER
#  include <intrin.h>
#else
#  include <x86intrin.h>
#endif

#pragma intrinsic(__rdtsc)

/// <summary>
	/// 一定時間の経過を確認するクラス
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
	/// コンストラクタ
	/// </summary>
	Timer() = default;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(ナノ秒)</param>
	Timer(const std::chrono::nanoseconds& _time) noexcept { type = Type::nanoseconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(マイクロ秒)</param>
	Timer(const std::chrono::microseconds& _time) noexcept { type = Type::microseconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(ミリ秒)</param>
	Timer(const std::chrono::milliseconds& _time) noexcept { type = Type::milliseconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(秒)</param>
	Timer(const std::chrono::seconds& _time) noexcept { type = Type::seconds; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(分)</param>
	Timer(const std::chrono::minutes& _time) noexcept { type = Type::minutes; time = _time.count(); }
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(時)</param>
	Timer(const std::chrono::hours& _time) noexcept { type = Type::hours; time = _time.count(); }

	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(ナノ秒)</param>
	void set(const std::chrono::nanoseconds& _time) noexcept { type = Type::nanoseconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(マイクロ秒)</param>
	void set(const std::chrono::microseconds& _time) noexcept { type = Type::microseconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(ミリ秒)</param>
	void set(const std::chrono::milliseconds& _time) noexcept { type = Type::milliseconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(秒)</param>
	void set(const std::chrono::seconds& _time) noexcept { type = Type::seconds; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(分</param>
	void set(const std::chrono::minutes& _time) noexcept { type = Type::minutes; time = _time.count(); }
	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(時)</param>
	void set(const std::chrono::hours& _time) noexcept { type = Type::hours; time = _time.count(); }

	/// <summary>
	/// タイマーを開始させる
	/// </summary>
	void start() noexcept { s = std::chrono::high_resolution_clock::now(); }

	/// <summary>
	/// 設定時間経過したかを得る
	/// </summary>
	/// <returns>経過していれば true, それ以外は false</returns>
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
	/// 設定時間経過したかを得る
	/// </summary>
	/// <returns>経過していれば true, それ以外は false</returns>
	operator bool() const noexcept { return check(); }

};

/// <summary>
/// 一定時間の経過を確認するクラス
/// </summary>
class MilliSecTimer {
private:

	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	unsigned long long int startCycle = 0;
	long long time = 0;

#ifdef _MSC_VER
	const double CyclePerMilliSec = 3200000.0;
#else
	const double CyclePerMilliSec = 2800000.0;//仮置き
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
	/// コンストラクタ
	/// </summary>
	MilliSecTimer() = default;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_time">設定時間(ミリ秒)</param>
	MilliSecTimer(const std::chrono::milliseconds& _time) noexcept { time = _time.count(); }

	/// <summary>
	/// 時間を設定する
	/// </summary>
	/// <param name="_time">設定時間(ミリ秒)</param>
	void set(const std::chrono::milliseconds& _time) noexcept { time = _time.count(); }

	/// <summary>
	/// タイマーを開始させる
	/// </summary>
	void start() noexcept {
		startCycle = getCycle();
	}

	/// <summary>
	/// 設定時間経過したかを得る
	/// </summary>
	/// <returns>経過していれば true, それ以外は false</returns>
	inline const bool check() const noexcept {
		return (getCycle() - startCycle) / CyclePerMilliSec >= time;
	}

	/// <summary>
	/// 設定時間経過したかを得る
	/// </summary>
	/// <returns>経過していれば true, それ以外は false</returns>
	operator bool() const noexcept { return check(); }

	/// <summary>
	/// 経過時間を取得する(ミリ秒)
	/// </summary>
	/// <returns>計測時間(ミリ秒)</returns>
	inline const long long interval() const noexcept {
		return static_cast<long long int>((getCycle() - startCycle) / CyclePerMilliSec);
	}

};

/// <summary>
	/// 時間計測を行うクラス
	/// </summary>
class Stopwatch {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Stopwatch() = default;

	/// <summary>
	/// 計測を開始する
	/// </summary>
	inline void start() noexcept {
		s = std::chrono::high_resolution_clock::now();
		e = s;
	}
	/// <summary>
	/// 計測を停止する
	/// </summary>
	inline void stop() noexcept {
		e = std::chrono::high_resolution_clock::now();
	}

	/// <summary>
	/// 計測時間を取得する(ナノ秒)
	/// </summary>
	/// <returns>計測時間(ナノ秒)</returns>
	inline const long long nanoseconds() const noexcept { return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count(); }
	/// <summary>
	/// 計測時間を取得する(マイクロ秒)
	/// </summary>
	/// <returns>計測時間(マイクロ秒)</returns>
	inline const long long microseconds() const noexcept { return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count(); }
	/// <summary>
	/// 計測時間を取得する(ミリ秒)
	/// </summary>
	/// <returns>計測時間(ミリ秒)</returns>
	inline const long long millisecond() const noexcept { return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count(); }
	/// <summary>
	/// 計測時間を取得する(秒)
	/// </summary>
	/// <returns>計測時間(秒)</returns>
	inline const long long second() const noexcept { return std::chrono::duration_cast<std::chrono::seconds>(e - s).count(); }
	/// <summary>
	/// 計測時間を取得する(分)
	/// </summary>
	/// <returns>計測時間(分)</returns>
	inline const long long minutes() const noexcept { return std::chrono::duration_cast<std::chrono::minutes>(e - s).count(); }
	/// <summary>
	/// 計測時間を取得する(時)
	/// </summary>
	/// <returns>計測時間(時)</returns>
	inline const long long hours() const noexcept { return std::chrono::duration_cast<std::chrono::hours>(e - s).count(); }

	/// <summary>
	/// 単位付きの計測時間の文字列を得る(ナノ秒)
	/// </summary>
	/// <returns>計測時間の文字列(ナノ秒)</returns>
	inline const std::string toString_ns() const { return std::to_string(nanoseconds()) + "ns"; }
	/// <summary>
	/// 単位付きの計測時間の文字列を得る(マイクロ秒)
	/// </summary>
	/// <returns>計測時間の文字列(マイクロ秒)</returns>
	inline const std::string toString_us() const { return std::to_string(microseconds()) + "us"; }
	/// <summary>
	/// 単位付きの計測時間の文字列を得る(ミリ秒)
	/// </summary>
	/// <returns>計測時間の文字列(ミリ秒)</returns>
	inline const std::string toString_ms() const { return std::to_string(millisecond()) + "ms"; }
	/// <summary>
	/// 単位付きの計測時間の文字列を得る(秒)
	/// </summary>
	/// <returns>計測時間の文字列(秒)</returns>
	inline const std::string toString_s() const { return std::to_string(second()) + "s"; }
	/// <summary>
	/// 単位付きの計測時間の文字列を得る(分)
	/// </summary>
	/// <returns>計測時間の文字列(分)</returns>
	inline const std::string toString_m() const { return std::to_string(minutes()) + "m"; }
	/// <summary>
	/// 単位付きの計測時間の文字列を得る(時)
	/// </summary>
	/// <returns>計測時間の文字列(時)</returns>
	inline const std::string toString_h() const { return std::to_string(hours()) + "h"; }

private:

	std::chrono::time_point<std::chrono::high_resolution_clock> s;
	std::chrono::time_point<std::chrono::high_resolution_clock> e;

};
