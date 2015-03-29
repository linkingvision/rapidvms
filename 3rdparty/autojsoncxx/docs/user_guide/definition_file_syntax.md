## Definition file syntax

The definition file is a simple JSON file that lists all the classes, their members as well as options to control the parsing. The root must be an object corresponding to a class definition, or an array of such objects.

### Class definition

A class definition is an object with the following fields:

* **name**. The unqualified name of the class.
* **namespace**. (optional). The full namespace of this class, such as `mycompany::data` or `::mycompany::data`. When not set, the class is put in the global namespace.
* **parse_mode** (optional). "strict" or otherwise, default "". When set to strict, any unrecognized JSON key will cause an `UnknownFieldError`. Otherwise they are simply ignored. The default is ignoring, so that you can upgrade your protocol by appending new fields without affecting old applications.
* **constructor_code**. (optional). Arbitrary C++ code to execute in the constructor, useful if you need to perform initialization that cannot be done with the `default` option.
* **comment** (optional). Ignored.
* **members**. An array of member definitions.

### Member definition

A member definition is a JSON array of two or three elements. The first two is "positional arguments", referring to the *fully qualified* type name and the variable name, mimicking the declaration order in C++. The third element, if any, is a JSON object with one or more of the following keys:

* **required**. true/false (default: false). When set to true, the lack of such field in the JSON triggers `MissingFieldError`. If you want to know whether a certain key is present but not wanting the errors, you can use a nullable wrapper.
* **default**. A boolean, number or string, used to initialize this field. When not set, the field is value initialized in the constructor.
* **json_key**. The corresponding key in JSON. When not set, it is the same as the variable name.
* **comment**. Ignored.
