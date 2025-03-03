#include "Node_menu.h"
#include "LinearSystems.h"

bool isOnlyOneLink(const std::vector<Pin>& Inputs)
{
    for (auto input : Inputs)
    {
        if (input.links.size() != 1)
            return false;
    }
    return true;
}

void SetOutputs(std::vector<Pin>::iterator output_it) {}

template<typename T, typename... Targs>
void SetOutputs(std::vector<Pin>::iterator output_it, T value, Targs... Fargs)
{
    output_it->Data = value;
    SetOutputs(++output_it, Fargs...);
}

void SetOutputsNull(std::vector<Pin>& Outputs)
{
    auto it = Outputs.begin();
    while (it != Outputs.end())
    {
        it->Data = NULL;
        it++;
    }
}

// ------

Node* NodeMenu::SpawnInputNode()
{
    node("Input", 120, 219, 226, NodeType::Blueprint);
    outputd("Matrix", PinDataType::Real_2d_array, (void*)(new alglib::real_2d_array()));
    outputd("Vector", PinDataType::Real_1d_array, (void*)(new alglib::real_1d_array()));
    outputd("Double", PinDataType::Double, (void*)(new double(0)));
    outputd("Integer", PinDataType::Int, (void*)(new int(0)));
    return build();
}

Node* NodeMenu::SpawnOutputNode()
{
    node("Output", 74, 255, 24, NodeType::Blueprint);
    inputd("Matrix", PinDataType::Real_2d_array);
    inputd("Vector", PinDataType::Real_1d_array);
    inputd("Double", PinDataType::Double);
    inputd("Integer", PinDataType::Int);
    return build();
}

Node* NodeMenu::SpawnMatrixMultiplicationNode()
{
    Node::ProcessOperation Multiplication = [](const std::vector<Pin>& Inputs, std::vector<Pin>& Outputs)
    {
        if (!isOnlyOneLink(Inputs))
        {
            SetOutputsNull(Outputs);
            return;
        }

        alglib::real_2d_array* M1 = (alglib::real_2d_array*)Inputs[0].Data;
        alglib::real_2d_array* M2 = (alglib::real_2d_array*)Inputs[1].Data;

        if (!M1 || !M2 || (M1->cols() != M2->rows()))
        {
            SetOutputsNull(Outputs);
            return;
        }

        alglib::real_2d_array* Res {new alglib::real_2d_array()};
        multiply_matrices(*M1, *M2, *Res);
        SetOutputs(Outputs.begin(), Res);
    };

    node("Matrix Multiplication", 74, 255, 24, NodeType::Blueprint, Multiplication);
    input("", PinDataType::Real_2d_array);
    input("", PinDataType::Real_2d_array);

    output("Value", PinDataType::Real_2d_array);
    return build();
}

Node* NodeMenu::SpawnGaussNode()
{
    Node::ProcessOperation operation = [](const std::vector<Pin>& Inputs, std::vector<Pin>& Outputs)
    {
        if (!isOnlyOneLink(Inputs))
        {
            SetOutputsNull(Outputs);
            return;
        }

        alglib::real_2d_array* A = (alglib::real_2d_array*)Inputs[0].Data;
        alglib::real_1d_array* b = (alglib::real_1d_array*)Inputs[1].Data;

        if (!A || !b || (A->cols() != A->rows()) || b->length() != A->cols()) // Проверка корректности данных
        {
            SetOutputsNull(Outputs);
            return;
        }

        alglib::real_1d_array* X {new alglib::real_1d_array()};
        double* det{ new double() };
        alglib::real_2d_array* L {new alglib::real_2d_array()};
        alglib::real_2d_array* U {new alglib::real_2d_array()};
        alglib::real_2d_array* Ai {new alglib::real_2d_array()};
        GaussMethod(*A,*b,*X,A->cols(), *det, *L, *U, *Ai);
        SetOutputs(Outputs.begin(), X, det, L, U, Ai);
    };

    node("Gauss Method", 74, 255, 24, NodeType::Blueprint, operation);
    input("Matrix A", PinDataType::Real_2d_array);
    input("Vector b", PinDataType::Real_1d_array);

    output("Vector X", PinDataType::Real_1d_array);
    output("Determinant", PinDataType::Double);
    output("Matrix L", PinDataType::Real_2d_array);
    output("Matrix U", PinDataType::Real_2d_array);
    output("Inversed matrix", PinDataType::Real_2d_array);
    return build();
}


Node* NodeMenu::SpawnQRNode()
{
    Node::ProcessOperation operation = [](const std::vector<Pin>& Inputs, std::vector<Pin>& Outputs)
    {
        if (!isOnlyOneLink(Inputs))
        {
            SetOutputsNull(Outputs);
            return;
        }

        alglib::real_2d_array* A = (alglib::real_2d_array*)Inputs[0].Data;
        int* steps_count = (int*)Inputs[1].Data;

        if (!A || !steps_count || (A->cols() != A->rows()) && *steps_count > 0)
        {
            SetOutputsNull(Outputs);
            return;
        }

        alglib::real_1d_array* eigenvalues = new alglib::real_1d_array();
        QRMethod(*A, A->rows(), *steps_count, *eigenvalues);
        SetOutputs(Outputs.begin(), eigenvalues);
    };

    node("QR Method", 74, 255, 24, NodeType::Blueprint, operation);
    input("Matrix A", PinDataType::Real_2d_array);
    input("Steps count", PinDataType::Int);

    output("Eigenvalues", PinDataType::Real_1d_array);
    return build();
}

Node* NodeMenu::SpawnSimpleIterationNode()
{
    Node::ProcessOperation operation = [](const std::vector<Pin>& Inputs, std::vector<Pin>& Outputs)
    {
        if (!isOnlyOneLink(Inputs))
        {
            SetOutputsNull(Outputs);
            return;
        }

        alglib::real_2d_array* A = (alglib::real_2d_array*)Inputs[0].Data;
        alglib::real_1d_array* X0 = (alglib::real_1d_array*)Inputs[1].Data;
        alglib::real_1d_array* b = (alglib::real_1d_array*)Inputs[2].Data;
        int* type = (int*)Inputs[3].Data;
        int* steps_count = (int*)Inputs[4].Data;

        int n;
        if (!A || (n = A->cols(), n != A->rows()) || !b || !steps_count || !type)
        {
            SetOutputsNull(Outputs);
            return;
        }

        alglib::real_1d_array* X = new alglib::real_1d_array();
        X->setlength(n);

        if (!X0) vector_identity(*X, n);
        else alglib::rvectorcopy(n, *X0, 0, *X, 0);

        SimpleIterationMethod(*A, *b, *X, (IterationsType)*type, *steps_count);

        SetOutputs(Outputs.begin(), X);
    };

    node("Simple Iteration Method", 74, 255, 24, NodeType::Blueprint, operation);
    input("Matrix A", PinDataType::Real_2d_array);
    input("Vector X0", PinDataType::Real_1d_array);
    input("Vector b", PinDataType::Real_1d_array);
    input("Method Type\n0: Gauss Zeidel\n1: Jacobi", PinDataType::Int);
    input("Steps count", PinDataType::Int);

    output("X", PinDataType::Real_1d_array);
    return build();
}