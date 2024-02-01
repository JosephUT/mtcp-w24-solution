#ifndef MROS_JSON_HPP
#define MROS_JSON_HPP

#include <map>
#include <string>
#include <variant>
#include <type_traits>
#include <vector>
#include <sstream>

/**
 * Simple JSON parser. Supports integer, double, bool, and std::string types.
 */
class Json {
public:
    /**
     * Value class that stores standard basic Json types, allowing with arrays formatted as std::vector.
     * Uses std::variant as underlying type.
     */
    class Value {
    public:
        friend class Json;

        /**
         * Base default constructor for Json::Value.
         */
        Value() = default;

        /**
         * Base default destructor for Json::Value.
         */
        ~Value() = default;

        /**
         * Copy constructor for Json::Value.
         * @param v The Value to be copied.
         */
        Value(Value const &v) = default;

        /**
         * Move constructor for Json::Value.
         * @param v The Value to be copied.
         */
        Value(Value &&v) = default;

        /**
         * Assignment operator for Json::Value.
         * @param v The Value to be copied.
         * @return The copied Value. This is erased before assignment.
         */
        Value &operator=(Value const &v) = default;

        /**
         * Equality operator for Json::Value.
         * @param v The Value to be compared. 
         * @return True if the Values are equal, false otherwise.
         */
        bool operator==(Json::Value const &v) const {
            return this->data_ == v.data_;
        }

        /**
         * Implicit value constructor for supported types defined by Json::Value::ValueType.
         * @tparam T The type of parameter v.
         * @param v Data of type T to to be stored by the constructed Value.
         */
        template<typename T, typename = std::enable_if_t<!std::is_same_v<T, Value>>>
        Value(T v) : data_(std::move(v)) {}

    private:
        /**
         * A std::variant<type...> alias for Json::Value.
         */
        using ValueType = std::variant<int, double, std::string, std::vector<int>, std::vector<double>, std::vector<std::string>>;

        /**
         * The underlying data member.
         */
        ValueType data_;

    public:
        /**
         * Get a reference to the data memeber. 
         * @tparam T The type of reference to get. Must be one of the types in ValueType.
         * @return A reference of type T to the data member.
         */
        template<typename T>
        requires std::is_constructible_v<T>
        T &get() {
            return std::get<T>(this->data_);
        }

        /**
         * Get a copy or const referene to the data member. 
         * @tparam T type of value. Must be one of the types in ValueType.
         * @return A copy or const reference to the data member.
         */
        template<typename T>
        requires std::is_constructible_v<T>
        T const &get() const {
            return std::get<T>(this->data_);
        }
    };

    /**
     * Default constructor for Json.
     */
    Json() = default;

    /**
     * Default destructor for Json.
     */
    ~Json() = default;

    /**
     * Default copy constructor for Json.
     * @param json The Json to be copied.
     */
    Json(Json const &json) = default;

    /**
     * Default move constructor for Json.
     * @param json The Json to be moved.
     */
    Json(Json &&json) = default;

    /**
     * Default assignment operator for Json.
     * @param json The Json to be assigned.
     * @return A copy of Json. This is cleared before assignment.
     */
    Json &operator=(Json const &json) = default;

    /**
     * Templated assignment operator for assigning a Json to a struct. Usable for any struct with a toJson function.
     * @tparam StructT The type of the struct. Requires a toJson function.
     * @param struct_t The struct to operate on.
     * @return A reference to a Json representation of the struct.
     */
    template<typename StructT>
    requires requires (Json& json, StructT const& struct_t) {
        {StructT::toJson(json, struct_t ) } -> std::same_as<void>;
    }
    Json &operator=(StructT const& struct_t) {
        StructT::toJson(*this, struct_t);
        return *this;
    }

    /**
     * Templated construction operator for constructing a struct from a Json. Usable for any struct with a fromJson function.
     * @tparam StructT The type of the struct. Requires a fromJson function.
     */
    template<typename StructT>
    requires requires (StructT& struct_t, Json const& json) {
        {StructT::fromJson(struct_t, json) } -> std::same_as<void>;
    }
    operator StructT() const {
        StructT result;
        StructT::fromJson(result, *this);
        return result;
    }

    /**
     * Modifiable Value reference to a Json::Value.
     * @param key String mapping to the returned Value.
     * @return The Value mapped to by the key.
     */
    Value &operator[](std::string const &key);

    /**
     * Copy or const reference to Json::Value.
     * @param key String mapping to the returned Value.
     * @return The Value mapped to by the key.
     */
    Value const &operator[](std::string const &key) const;

    /**
     * Equality operator for Json.
     * @param json Json to compare with this.
     * @return True if all elements match, false otherwise.
     */
    bool operator==(Json const &json) const;

    /**
     * Convert Json object to string in JSON format.
     * @return The string representation of the Json.
     */
    std::string toString() const;

    /**
     * Set a Json object from a string in JSON format.
     * @param str Json formatted string.
     * @return Json object.
     */
    static Json fromString(std::string const &str);

protected:
    /**
     * Map string -> Json::Value.
     */
    std::map<std::string, Value> elts_;

private:
    /**
     * Convert Json::value to string. Helper function for Json::toString.
     * @param value The Value to convert to a string.
     * @return A JSON formatted string representation of the Value.
     */
    static std::string stringify(Json::Value const &value);

    /**
     * Helper function to take string and convert to Json::Value type
     * @param str string of value
     * @return Json::Value with proper type
     */
    static Json::Value getType(std::string const& str);

    /**
     * Sample second implementation of Json::getType
     * @param str string of value
     * @return Json::Value with proper type
     */
    static Json::Value getType2(std::string const& str);
};

#endif