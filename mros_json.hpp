#include <map>
#include <string>
#include <variant>
#include <type_traits>
#include <vector>
#include <sstream>

/**
 * Simple JSON parser. Supports integer, double, bool, and std::string types
 */
class Json {
public:
    /**
     * Value class that stores standard basic Json types, allowing with arrays formatted as std::vector
     * uses std::variant as underlying type
     * similar to EECS 281 LogMan
     */
    class Value {
    public:
        friend class Json;

        /**
         * Base default constructor for Json::Value
         */
        Value() = default;

        /**
         * Base default destructor for Json::Value
         */
        ~Value() = default;

        /**
         * Copy constructor for Json::Value
         * @param v value to be copied
         */
        Value(Value const &v) = default;

        /**
         * Move constructor for Json::Value
         * @param v
         */
        Value(Value &&v) = default;

        /**
         * Assignment operator for Json::Value
         * @param v Value to be copied
         * @return copied json value. This is erased before assignment
         */
        Value &operator=(Value const &v) = default;

        bool operator==(Json::Value const &value) const {
            return this->data_ == value.data_;
        }

        /**
         * Implicit value constructor for supported types defined by Json::Value::ValueType
         * @tparam T Type of value constructed
         * @param val value of type T
         */
        template<typename T, typename = std::enable_if_t<!std::is_same_v<T, Value>>>
        Value(T val) : data_(std::move(val)) {}

    private:
        /**
         * std::variant<type...> alias for Json::Value
         */
        using ValueType = std::variant<int, double, std::string, std::vector<int>, std::vector<double>, std::vector<std::string>>;

        /**
         * Underlying data constructor
         */
        ValueType data_;

    public:

        /**
         * get reference to value stored. Must be one of the types in ValueType
         * @tparam T type of value
         * @return value of type T stored in data_
         */
        template<typename T>
        requires std::is_constructible_v<T>
        T &get() {
            return std::get<T>(this->data_);
        }

        /**
         * Get copy or const referene to value stored. Must be one of the types in ValueType
         * @tparam T type of value
         * @return
         */
        template<typename T>
        requires std::is_constructible_v<T>
        T const &get() const {
            return std::get<T>(this->data_);
        }

    };

    /**
     * Default constructor for Json
     */
    Json() = default;

    /**
     * Default destructor for Json
     */
    ~Json() = default;

    /**
     * Default copy constructor for Json
     * @param json json to be copied
     */
    Json(Json const &json) = default;

    /**
     * Default move constructor for Json
     * @param json json to be moved
     */
    Json(Json &&json) = default;

    /**
     * Default assignment operator for Json
     * @param json json to be assigned
     * @return copy of json. This is cleared before assignment
     */
    Json &operator=(Json const &json) = default;

    /**
     * Modifiable value reference to a Json::Value
     * @param key string key name
     * @return Json::Value reference
     */
    Value &operator[](std::string const &key);

    /**
     * Copy or const reference to Json::Value
     * @param key string key name
     * @return Json::Value
     */
    Value const &operator[](std::string const &key) const;

    bool operator==(Json const &json) const;

    /**
     * Converts Json object to string
     * @return string json representation
     */
    std::string toString() const;

    /**
     * Converts json string to Json object
     * @param str Json formatted string
     * @return Json object
     */
    static Json fromString(std::string const &str);

protected:
    /**
     * Map string -> Json::Value
     */
    std::map<std::string, Value> elts_;
private:
    /**
     * Helper function to convert Json::value to string. Used in Json::toString
     * @param value Json::Value
     * @return string of type stored in Json::Value::ValueType
     */
    static std::string stringify(Json::Value const &value);
};