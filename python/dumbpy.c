#include <Python.h>
#include <structmember.h>
#include <math.h>
#include "../naive/matrix.h"


/*
 * Defines the struct that represents the object
 * Has the default PyObject_HEAD so it can be a python object
 * It also has the matrix that is being wrapped
 */
typedef struct {
    PyObject_HEAD
    matrix* mat;
} Matrix61c;

static PyTypeObject Matrix61cType;

// This is a helper function to be passed into apply func
float sigmoid(float x) {
    return 1 / (1 + expf(-1 * x));
}


/*
 * Destroy's the struct
 */
static void
Matrix61c_dealloc(Matrix61c* self) {
    free_matrix(self->mat);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

/*
 * Creates and allocates space for the struct
 */
static PyObject *
Matrix61c_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    Matrix61c *self;

    self = (Matrix61c *)type->tp_alloc(type, 0);

    return (PyObject *)self;
}

/*
 * Initializes the data in the struct
 */
static int
Matrix61c_init(Matrix61c *self, PyObject *args, PyObject *kwds) {
    PyObject *lst=NULL;

    static char *kwlist[] = {"", NULL};
    
    if (! PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &lst)) {
        PyErr_SetString(PyExc_TypeError, "Not given an object to initialize");
        return -1;
    }

    if (! PyList_Check(lst)) {
        PyErr_SetString(PyExc_TypeError, "Not given a list to initialize");
        return -1;
    }

    int rows = PyList_Size(lst);
    int i;
    
    if (rows <= 0) { // If there is an empty list given, return an empty matrix
        if (self != NULL) {
            if (allocate_matrix(&(self->mat), 0, 0) == -1) {
                PyErr_SetString(PyExc_TypeError, "Failed to allocate");
                return -1;
            }
            return 0;
        }
    }

    PyObject* item = PyList_GetItem(lst, 0);

    if (PyNumber_Check(item)) { // If we have an float, we should treat this as a row vector
        if (allocate_matrix(&(self->mat), rows, 1) == -1) {
            PyErr_SetString(PyExc_TypeError, "Failed to allocate");
            return -1;
        }
        for (i = 0; i < rows; i++) {
            item = PyList_GetItem(lst, i);
            if (! PyNumber_Check(item)) {
                free(self->mat);
                PyErr_SetString(PyExc_TypeError, "Items are not numbers");
                return -1;
            }
            set_loc(self->mat, i, 0, (float)PyFloat_AsDouble(PyNumber_Float(item)));
        }
    } else if (PyList_Check(item)) { // If we have list, treat this as a matrix or column vector
        int cols = PyList_Size(item); // Get the number of columsn
        if (allocate_matrix(&(self->mat), rows, cols) == -1) { // If we can't allocate space, fail
            PyErr_SetString(PyExc_TypeError, "Failed to allocate");
            return -1;
        }
        int j;
        PyObject* sub_item;
        for (i = 0; i < rows; i++) { // For each row
            item = PyList_GetItem(lst, i); // Get the item
            if (! PyList_Check(item)) { // Check its a list
                free(self->mat);
                PyErr_SetString(PyExc_TypeError, "Items are not lists");
                return -1;
            }
            if (cols != PyList_Size(item)) { // Check that it has the same number of items
                free(self->mat);
                PyErr_SetString(PyExc_TypeError, "Size is not correct");
                return -1;
            }
            for (j = 0; j < cols; j++) { // Loop through the items in the list
                sub_item = PyList_GetItem(item, j); // Get the item from the list
                if (! PyNumber_Check(sub_item)) { // Check its a valid item
                    PyErr_SetString(PyExc_TypeError, "Sub items are not numbers");
                    free(self->mat);
                    return -1;
                }
                set_loc(self->mat, i, j, (float)PyFloat_AsDouble(PyNumber_Float(sub_item))); // Place it in the matrix
            }
        }
    } else { // Otherwise error out
        PyErr_SetString(PyExc_TypeError, "List is not valid");
        return -1;
    }

    return 0;
}

/*
 * Returns a string that represents the matrix
 */
static PyObject *
Matrix61c_repr(Matrix61c *self) {
    int r, c; // Get the numer of rows and columns
    r = get_rows(self->mat);
    c = get_cols(self->mat);
    float* arr = malloc(r * c * sizeof(int)); // Get the matrix as an array
    get_matrix_as_array(arr, self->mat);
    // Convert the matrix into list of lists
    PyObject* rv = PyList_New(r);
    int i, j;
    for (i = 0; i < r; i++) {
        PyObject* new_list = PyList_New(c);
        for (j = 0; j < c; j++) {
            PyList_SET_ITEM(new_list, j, Py_BuildValue("f", arr[c * i + j]));
        }
        PyList_SET_ITEM(rv, i, new_list);
    }
    free(arr);
    PyObject* real_rv = PyObject_Repr(rv);
    Py_DECREF(rv);
    return real_rv;
}


/*
 * Returns a list that represents the matrix
 */
static PyObject *
Matrix61c_to_list(Matrix61c *self) {
    int r, c; // Get the numer of rows and columns
    r = get_rows(self->mat);
    c = get_cols(self->mat);
    float* arr = malloc(r * c * sizeof(int)); // Get the matrix as an array
    get_matrix_as_array(arr, self->mat);

    // Convert the matrix into list of lists
    PyObject* rv = PyList_New(r);
    int i, j;
    for (i = 0; i < r; i++) {
        PyObject* new_list = PyList_New(c);
        for (j = 0; j < c; j++) {
            PyList_SET_ITEM(new_list, j, Py_BuildValue("f", arr[c * i + j]));
        }
        PyList_SET_ITEM(rv, i, new_list);
    }

    free(arr);
    return rv;
}


static PyMemberDef Matrix61c_members[] = {
    {"mat", T_OBJECT_EX, offsetof(Matrix61c, mat), 0,
     "matrix"},
    {NULL}  /* Sentinel */
};

/* Methods of the Matrix class
 * Follows format:
 * Matrix61c_{name of method}
 */
static PyObject *
Matrix61c_scale(Matrix61c* self, PyObject* args) {
    float scale_amt;
    if (! PyFloat_Check(args)) {
        if (! PyArg_ParseTuple(args, "f", &scale_amt)) {
            PyErr_SetString(PyExc_TypeError, "You can only scale by a float");
            return NULL;
        }
    } else {
        scale_amt = PyLong_AsLong(args);
    }
    Matrix61c* rv = (Matrix61c*) Matrix61c_new(&Matrix61cType, NULL, NULL);
    allocate_matrix(&rv->mat, get_rows(self->mat), get_cols(self->mat));
    matrix_scale(self->mat, scale_amt, rv->mat);
    return (PyObject*)rv;
}

static PyObject *
Matrix61c_power(Matrix61c* self, PyObject* args) {
    int pwr_amt;
    if (! PyLong_Check(args)) {
        if (! PyArg_ParseTuple(args, "i", &pwr_amt)) {
            PyErr_SetString(PyExc_TypeError, "You can only take the power by an Integer");
            return NULL;
        }
    } else {
        pwr_amt = PyLong_AsLong(args);
    }
    Matrix61c* rv = (Matrix61c*) Matrix61c_new(&Matrix61cType, NULL, NULL);
    allocate_matrix(&rv->mat, get_rows(self->mat), get_cols(self->mat));
    matrix_power(self->mat, pwr_amt, rv->mat);
    return (PyObject*)rv;
}

static PyObject*
Matrix61c_uscore_power(Matrix61c* self, PyObject* pow, PyObject* mod) {
    return Matrix61c_power(self, pow);
}

static PyObject *
Matrix61c_add(Matrix61c* self, PyObject* args) {
    Matrix61c* other_mat;
    if (! PyObject_TypeCheck(args, &Matrix61cType)) {
        if (! PyArg_ParseTuple(args, "O", &other_mat)) {
            PyErr_SetString(PyExc_TypeError, "Dumbpy.matrix does not support '+' with other types");
            return NULL;
        }
        if (! PyObject_TypeCheck(other_mat, &Matrix61cType)) {
            PyErr_SetString(PyExc_TypeError, "Dumbpy.matrix does not support '+' with other types");
            return NULL;
        }
    } else {
        other_mat = (Matrix61c*)args;
    }
    Matrix61c* rv = (Matrix61c*) Matrix61c_new(&Matrix61cType, NULL, NULL);
    allocate_matrix(&rv->mat, get_rows(self->mat), get_cols(self->mat));
    matrix_add(self->mat, other_mat->mat, rv->mat);
    return (PyObject*)rv;
}

static PyObject *
Matrix61c_sub(Matrix61c* self, PyObject* args) {
    Matrix61c* other_mat;
    if (! PyObject_TypeCheck(args, &Matrix61cType)) {
        if (! PyArg_ParseTuple(args, "O", &other_mat)) {
            PyErr_SetString(PyExc_TypeError, "Dumbpy.matrix does not support '-' with other types");
            return NULL;
        }
        if (! PyObject_TypeCheck(other_mat, &Matrix61cType)) {
            PyErr_SetString(PyExc_TypeError, "Dumbpy.matrix does not support '-' with other types");
            return NULL;
        }
    } else {
        other_mat = (Matrix61c*)args;
    }
    Matrix61c* rv = (Matrix61c*) Matrix61c_new(&Matrix61cType, NULL, NULL);
    matrix* tmp;
    allocate_matrix(&tmp, get_rows(self->mat), get_cols(self->mat));
    allocate_matrix(&rv->mat, get_rows(self->mat), get_cols(self->mat));
    matrix_scale(other_mat->mat, -1, tmp);
    matrix_add(self->mat, tmp, rv->mat);
    free_matrix(tmp);
    return (PyObject*)rv;
}

static PyObject *
Matrix61c_multiply(Matrix61c* self, PyObject* args) {
    Matrix61c* other_mat;
    if (! PyObject_TypeCheck(args, &Matrix61cType)) {
        if (! PyArg_ParseTuple(args, "O", &other_mat)) {
            PyErr_SetString(PyExc_TypeError, "Dumbpy.matrix does not support '*' with other types");
            return NULL;
        }
        if (! PyObject_TypeCheck(other_mat, &Matrix61cType)) {
            PyErr_SetString(PyExc_TypeError, "Dumbpy.matrix does not support '*' with other types");
            return NULL;
        }
    } else {
        other_mat = (Matrix61c*)args;
    }
    Matrix61c* rv = (Matrix61c*) Matrix61c_new(&Matrix61cType, NULL, NULL);
    allocate_matrix(&rv->mat, get_rows(self->mat), get_cols(other_mat->mat));
    matrix_multiply(self->mat, other_mat->mat, rv->mat);
    return (PyObject*)rv;
}

static PyObject *
Matrix61c_uscore_multiply(Matrix61c* self, PyObject* args) {
    if (PyObject_TypeCheck(args, &Matrix61cType)) {
        return Matrix61c_multiply(self, args);
    } else if (PyFloat_Check(args)) {
        return Matrix61c_scale(self, args);
    }
    PyErr_SetString(PyExc_TypeError, "Dumbpy.matrix only supports '*' with floats and other matricies");
    return NULL;
}


static PyObject *
Matrix61c_dot(Matrix61c* self, PyObject* args) {
    Matrix61c* other_mat;
    if (! PyArg_ParseTuple(args, "O", &other_mat)) {
        PyErr_SetString(PyExc_TypeError, "");
        return NULL;
    }
    if (! PyObject_TypeCheck(other_mat, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "Dumbpy.matrix does not support dot with other types");
        return NULL;
    }
    if (get_rows(self->mat) != get_rows(other_mat->mat) || get_cols(self->mat) != 1 || get_cols(other_mat->mat) != 1) {
        PyErr_SetString(PyExc_TypeError, "Dot product can only be of row vectors");
        return NULL;
    }
    float rv;
    dot_product(self->mat, other_mat->mat, &rv);
    return PyFloat_FromDouble((double)rv);
}

static PyObject *
Matrix61c_transpose(Matrix61c *self) {
    Matrix61c* rv = (Matrix61c*) Matrix61c_new(&Matrix61cType, NULL, NULL);
    allocate_matrix(&rv->mat, get_cols(self->mat), get_rows(self->mat));
    matrix_transpose(self->mat, rv->mat);
    return (PyObject*)rv;
}

static PyObject *
Matrix61c_set_value(Matrix61c *self, PyObject* args) {
    int row, col;
    float val;
    if (! PyArg_ParseTuple(args, "iif", &row, &col, &val)) {
        //PyErr_SetString(PyExc_TypeError, "You can only index by integers and must provide a float as the value");
        return NULL;
    }
    if (row < 0 || row >= get_rows(self->mat) || col < 0 || col >= get_cols(self->mat)) {
        PyErr_SetString(PyExc_TypeError, "Index out of bounds");
        return NULL;
    }
    set_loc(self->mat, row, col, val);
    Py_RETURN_NONE;
}

static PyObject *
Matrix61c_get_value(Matrix61c *self, PyObject* args) {
    int row, col;
    if (! PyArg_ParseTuple(args, "ii", &row, &col)) {
        PyErr_SetString(PyExc_TypeError, "You can only index by integers and must provide a float as the value");
        return NULL;
    }
    if (row < 0 || row >= get_rows(self->mat) || col < 0 || col >= get_cols(self->mat)) {
        PyErr_SetString(PyExc_TypeError, "Index out of bounds");
        return NULL;
    }
    return PyFloat_FromDouble(get_loc(self->mat, row, col));
}

static PyObject *
Matrix61c_resize(Matrix61c *self, PyObject* args) {
    int row, col;
    if (! PyArg_ParseTuple(args, "ii", &row, &col)) {
        PyErr_SetString(PyExc_TypeError, "You must resize to interger lengths for rows and columns");
        return NULL;
    }
    if (row < 0 || col < 0 ) {
        PyErr_SetString(PyExc_TypeError, "Integers must be positive");
        return NULL;
    }
    int self_rows = get_rows(self->mat);
    int self_cols = get_cols(self->mat);

    Matrix61c* rv = (Matrix61c*) Matrix61c_new(&Matrix61cType, NULL, NULL);
    float* src = malloc(sizeof(float) * self_rows * self_cols);
    float* dst = malloc(sizeof(float) * row * col);
    get_matrix_as_array(src, self->mat);
    int i;
    int min_col = (self_cols > col) ? col : self_cols;
    for (i = 0; i < row; i++) {
        if (i >= self_rows) {
            memset(dst + i * col, 0, col);
        } else {
            memcpy(dst + i * col, src + i * self_cols, min_col);
            if (min_col < col) {
                memset(dst + i * col + min_col, 0, col - min_col);
            }
        }
    }

    rv->mat = arr_to_matrix(dst, row, col);
    free(src);
    free(dst);

    return (PyObject*)rv;
}

static PyObject *
Matrix61c_get_rows(Matrix61c *self) {
    return PyLong_FromLong((long)get_rows(self->mat));
}

static PyObject *
Matrix61c_get_cols(Matrix61c *self) {
    return PyLong_FromLong((long)get_cols(self->mat));
}

static PyObject *
Matrix61c_ones(Matrix61c *cls, PyObject* args) {
    int row, col;
    if (! PyArg_ParseTuple(args, "ii", &row, &col)) {
        PyErr_SetString(PyExc_TypeError, "You must provide interger lengths for rows and columns");
        return NULL;
    }
    if (row < 1 || col < 1 ) {
        PyErr_SetString(PyExc_TypeError, "Integer lengths must be positive and non-zero");
        return NULL;
    }
    Matrix61c* rv = (Matrix61c*) Matrix61c_new(&Matrix61cType, NULL, NULL);
    allocate_matrix(&rv->mat, row, col);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            set_loc(rv->mat, i, j, 1.0);
        }
    }
    return (PyObject*)rv;
}

static PyObject *
Matrix61c_row(Matrix61c *self, PyObject* args) {
    int row;
    if (! PyArg_ParseTuple(args, "i", &row)) {
        PyErr_SetString(PyExc_TypeError, "You can only index by integers and must provide a float as the value");
        return NULL;
    }
    if (row < 0 || row >= get_rows(self->mat)) {
        PyErr_SetString(PyExc_TypeError, "Index out of bounds");
        return NULL;
    }
    int col = get_cols(self->mat);
    Matrix61c* rv = (Matrix61c*) Matrix61c_new(&Matrix61cType, NULL, NULL);
    allocate_matrix(&rv->mat, 1, col);
    float val;
    for (int j = 0; j < col; j++) {
        val = get_loc(self->mat, row, j);
        set_loc(rv->mat, 0, j, val);
    }
    return (PyObject*)rv;
}

static PyObject *
Matrix61c_tanh(Matrix61c *self) {
    Matrix61c* rv = (Matrix61c*) Matrix61c_new(&Matrix61cType, NULL, NULL);
    allocate_matrix(&rv->mat, get_rows(self->mat), get_cols(self->mat));
    apply_func(self->mat, rv->mat, tanhf);
    return (PyObject*)rv;
}

static PyObject *
Matrix61c_sigmoid(Matrix61c *self) {
    Matrix61c* rv = (Matrix61c*) Matrix61c_new(&Matrix61cType, NULL, NULL);
    allocate_matrix(&rv->mat, get_rows(self->mat), get_cols(self->mat));
    apply_func(self->mat, rv->mat, sigmoid);
    return (PyObject*)rv;
}

static PyObject *
Matrix61c_ele_mul(Matrix61c *self, PyObject* args) {
    Matrix61c* other_mat;
    if (! PyArg_ParseTuple(args, "O", &other_mat)) {
        PyErr_SetString(PyExc_TypeError, "");
        return NULL;
    }
    if (! PyObject_TypeCheck(other_mat, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "Numc.matrix does not support element_mult with other types");
        return NULL;
    }
    if (get_rows(self->mat) != get_rows(other_mat->mat) || get_cols(self->mat) != get_cols(other_mat->mat)) {
        PyErr_SetString(PyExc_TypeError, "Element multiply must be of two same sized matricies");
        return NULL;
    }
    Matrix61c* rv = (Matrix61c*) Matrix61c_new(&Matrix61cType, NULL, NULL);
    allocate_matrix(&rv->mat, get_rows(self->mat), get_cols(other_mat->mat));
    matrix_multiply_elementwise(self->mat, other_mat->mat, rv->mat);
    return (PyObject*)rv;
}

static PyObject *
Matrix61c_outer(Matrix61c *self, PyObject* args) {
    Matrix61c* other_mat;
    if (! PyArg_ParseTuple(args, "O", &other_mat)) {
        PyErr_SetString(PyExc_TypeError, "");
        return NULL;
    }
    if (! PyObject_TypeCheck(other_mat, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "Numc.matrix does not support outer with other types");
        return NULL;
    }
    if (get_cols(self->mat) != 1 || get_cols(other_mat->mat) != 1) {
        PyErr_SetString(PyExc_TypeError, "Outer product can only be of row vectors");
        return NULL;
    }
    Matrix61c* rv = (Matrix61c*) Matrix61c_new(&Matrix61cType, NULL, NULL);
    allocate_matrix(&rv->mat, get_rows(self->mat), get_rows(other_mat->mat));
    outer_product(self->mat, other_mat->mat, rv->mat);
    return (PyObject*)rv;
}

/* Defines all of the methods of the matrix*/
static PyMethodDef Matrix61c_methods[] = {
    {"scale", (PyCFunction)Matrix61c_scale, METH_VARARGS,
     "Scales the matrix by `amt`"},
    {"power", (PyCFunction)Matrix61c_power, METH_VARARGS,
    "Raises the matrix to the `amt` power"},
    {"add", (PyCFunction)Matrix61c_add, METH_VARARGS,
    "Adds two matricies together"},
    {"sub", (PyCFunction)Matrix61c_sub, METH_VARARGS,
    "Subtracts one matrix from another"},
    {"multiply", (PyCFunction)Matrix61c_multiply, METH_VARARGS,
    "Multiplies two matricies together"},
    {"to_list", (PyCFunction)Matrix61c_to_list, METH_NOARGS,
    "Returns a list that represents the matrix"},
    {"dot", (PyCFunction)Matrix61c_dot, METH_VARARGS,
    "Returns a float of the dot product of two row vectors"},
    {"element_mult", (PyCFunction)Matrix61c_ele_mul, METH_VARARGS,
    "Multiplies element by element in the matricies provided"},
    {"outer", (PyCFunction)Matrix61c_outer, METH_VARARGS,
    "Performs the outer product of two vectors"},
    {"transpose", (PyCFunction)Matrix61c_transpose, METH_NOARGS,
    "Returns transpose of the matrix"},
    {"set", (PyCFunction)Matrix61c_set_value, METH_VARARGS,
    "Sets the value at location (row, col) to val"},
    {"get", (PyCFunction)Matrix61c_get_value, METH_VARARGS,
    "Gets the value at location (row, col) to val"},
    {"resize", (PyCFunction)Matrix61c_resize, METH_VARARGS,
    "Returns a new matrix with the new size, filled with zeros if made larger"},
    {"getRow", (PyCFunction)Matrix61c_row, METH_VARARGS,
    "Returns a row of the matrix as a new matrix"},
    {"tanh", (PyCFunction)Matrix61c_tanh, METH_NOARGS,
    "Returns a matrix with the function applied to each element"},
    {"sigmoid", (PyCFunction)Matrix61c_sigmoid, METH_NOARGS,
    "Returns a matrix with the function applied to each element"},
    {"get_rows", (PyCFunction)Matrix61c_get_rows, METH_NOARGS,
    "Returns the number of rows in the matrix"},
    {"get_cols", (PyCFunction)Matrix61c_get_cols, METH_NOARGS,
    "Returns the number of columns in the matrix"},
    {"ones", (PyCFunction)Matrix61c_ones, METH_VARARGS | METH_CLASS,
    "Returns a new matrix with dimensions of (row, col), filled with ones"},
    {NULL}  /* Sentinel */
};

// Declare the comparision before the struct to use it, then implement after
static PyObject *
Matrix61c_richcompare(Matrix61c *a, Matrix61c *b, int op);

static PyNumberMethods Matrix61c_as_number = {
   (binaryfunc)Matrix61c_add, // binaryfunc nb_add;
   (binaryfunc)Matrix61c_sub, // binaryfunc nb_subtract;
   (binaryfunc)Matrix61c_uscore_multiply, // binaryfunc nb_multiply;
   0, // binaryfunc nb_remainder;
   0, // binaryfunc nb_divmod;
   (ternaryfunc)Matrix61c_uscore_power, // ternaryfunc nb_power;
   0, // unaryfunc nb_negative;
   0, // unaryfunc nb_positive;
   0, // unaryfunc nb_absolute;
   0, // inquiry nb_bool;
   0, // unaryfunc nb_invert;
   0, // binaryfunc nb_lshift;
   0, // binaryfunc nb_rshift;
   0, // binaryfunc nb_and;
   0, // binaryfunc nb_xor;
   0, // binaryfunc nb_or;
   0, // unaryfunc nb_int;
   0, // void *nb_reserved;
   0, // unaryfunc nb_float;

   0, // binaryfunc nb_inplace_add;
   0, // binaryfunc nb_inplace_subtract;
   0, // binaryfunc nb_inplace_multiply;
   0, // binaryfunc nb_inplace_remainder;
   0, // ternaryfunc nb_inplace_power;
   0, // binaryfunc nb_inplace_lshift;
   0, // binaryfunc nb_inplace_rshift;
   0, // binaryfunc nb_inplace_and;
   0, // binaryfunc nb_inplace_xor;
   0, // binaryfunc nb_inplace_or;

   0, // binaryfunc nb_floor_divide;
   0, // binaryfunc nb_true_divide;
   0, // binaryfunc nb_inplace_floor_divide;
   0, // binaryfunc nb_inplace_true_divide;

   0, // unaryfunc nb_index;

   0, // binaryfunc nb_matrix_multiply;
   0, // binaryfunc nb_inplace_matrix_multiply;
};

static PyTypeObject Matrix61cType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "dumbpy.Matrix",             /* tp_name */
    sizeof(Matrix61c),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Matrix61c_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    (reprfunc)Matrix61c_repr,            /* tp_repr */
    &Matrix61c_as_number,                /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "dumbpy.Matrix objects",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    (richcmpfunc)Matrix61c_richcompare, /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Matrix61c_methods,             /* tp_methods */
    Matrix61c_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Matrix61c_init,      /* tp_init */
    0,                         /* tp_alloc */
    Matrix61c_new,                 /* tp_new */
};

static PyObject *
Matrix61c_richcompare(Matrix61c *a, Matrix61c *b, int op) {
    if (op == Py_NE || op == Py_EQ) {
        if (PyObject_TypeCheck(b, &Matrix61cType)) {
            if (get_rows(a->mat) != get_rows(b->mat) || get_cols(a->mat) != get_cols(b->mat)) {
                return op == Py_EQ ? Py_False : Py_True;
            }
            int i,j;
            for (i = 0; i < get_rows(a->mat); i++) {
                for (j = 0; j < get_cols(a->mat); j++) {
                    if (get_loc(a->mat, i, j) != get_loc(b->mat, i, j) && op == Py_EQ) {
                        return Py_False;
                    } else if (get_loc(a->mat, i, j) == get_loc(b->mat, i, j) && op == Py_NE) {
                        return Py_False;
                    } 
                }
            }
            return Py_True;
        } else {
            if (op == Py_NE) {
                PyErr_SetString(PyExc_TypeError, "'!=' not supported between Dumbpy.matrix and other types");
            } else {
                PyErr_SetString(PyExc_TypeError, "'==' not supported between Dumbpy and other types");
            }
            return NULL;
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "Dumbpy only supports '=='' and '!='");
        return NULL;
    }
}

static PyModuleDef dumbpymodule = {
    PyModuleDef_HEAD_INIT,
    "matrix",
    "A numpy like matrix",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_dumbpy(void) {
    PyObject* m;

    if (PyType_Ready(&Matrix61cType) < 0)
        return NULL;

    m = PyModule_Create(&dumbpymodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&Matrix61cType);
    PyModule_AddObject(m, "Matrix", (PyObject *)&Matrix61cType);
    return m;
}
