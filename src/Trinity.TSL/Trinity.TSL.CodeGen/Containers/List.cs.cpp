#include "common.h"
#include <string>
#include <SyntaxNode.h>

using std::string;

namespace Trinity
{
    namespace Codegen
    {
        string* 
List(
NFieldType* node)
        {
            string* source = new string();
            
source->append(R"::(
namespace )::");
source->append(Codegen::GetString(Trinity::Codegen::GetNamespace()));
source->append(R"::(
{
    )::");
int element_len_1 = node->listElementType->type_size();
bool element_fixed_1 = (element_len_1 > -1);
bool element_need_accessor_1 = data_type_need_accessor(node->listElementType);
source->append(R"::(
    public unsafe class )::");
source->append(Codegen::GetString(data_type_get_accessor_name(node)));
source->append(R"::( : IEnumerable<)::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::(>
    {
        internal byte* CellPtr;
        internal long? CellID;
        ResizeFunctionDelegate ResizeFunction;
        internal )::");
source->append(Codegen::GetString(data_type_get_accessor_name(node)));
source->append(R"::((byte* _CellPtr, ResizeFunctionDelegate func)
        {
            CellPtr = _CellPtr;
            ResizeFunction = func;
            CellPtr += 4;
        }
        internal int length
        {
            get
            {
                return *(int*)(CellPtr - 4);
            }
        }
        )::");
if (element_need_accessor_1)
{
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::( elementAccessor;
        )::");
}
source->append(R"::(
        /// <summary>
        /// Gets the number of elements actually contained in the List. 
        /// </summary>";
        public unsafe int Count
        {
            get
            {
                )::");
if (element_fixed_1)
{
source->append(R"::(
                return length / )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::(;
                )::");
}
else
{
source->append(R"::(
                byte* targetPtr = CellPtr;
                byte* endPtr = CellPtr + length;
                int ret = 0;
                while (targetPtr < endPtr)
                {
                    ++ret;
                }
                return ret;
                )::");
}
source->append(R"::(
            }
        }
        /// <summary>
        /// Gets or sets the element at the specified index. 
        /// </summary>
        /// <param name="index">Given index</param>
        /// <returns>Corresponding element at the specified index</returns>";
        public unsafe )::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::( this[int index]
        {
            get
            {
                )::");
if (!element_need_accessor_1)
{
source->append(R"::(
                return *()::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::(*)(CellPtr + index * )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::();
                )::");
}
else
{
source->append(R"::(
                {
                    )::");
if (element_fixed_1)
{
source->append(R"::(
                    elementAccessor.CellPtr = (this.CellPtr + index * )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::();
                    elementAccessor.CellID = this.CellID;
                    )::");
}
else
{
source->append(R"::(
                    {
                        byte* targetPtr = CellPtr;
                        while (index-- > 0)
                        {
                        }
                        )::");
if (node->listElementType->is_string() || node->listElementType->is_list())
{
source->append(R"::(
                        elementAccessor.CellPtr = targetPtr + 4;
                        )::");
}
else
{
source->append(R"::(
                        elementAccessor.CellPtr = targetPtr;
                        elementAccessor.CellID = this.CellID;
                        )::");
}
source->append(R"::(
                    }
                    )::");
}
source->append(R"::(
                    return elementAccessor;
                }
                )::");
}
source->append(R"::(
            }
            set
            {
                )::");
if (!element_need_accessor_1)
{
source->append(R"::(
                *()::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::(*)(CellPtr + index * )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::() = value;
                )::");
}
else
{
source->append(R"::(
                {
                    if ((object)value == null) throw new ArgumentNullException("The assigned variable is null.");
                    elementAccessor.CellID = this.CellID;
                    byte* targetPtr = CellPtr;
                    )::");
if (element_fixed_1)
{
source->append(R"::(
                    targetPtr += (index * )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::();
                    )::");
}
else
{
source->append(R"::(
                    while (index-- > 0)
                    {
                    }
                    )::");
}
source->append(R"::(
                }
                )::");
}
source->append(R"::(
            }
        }
        /// <summary>
        /// Copies the elements to a new byte array
        /// </summary>
        /// <returns>Elements compactly arranged in a byte array.</returns>
        public unsafe byte[] ToByteArray()
        {
            byte[] ret = new byte[length];
            fixed (byte* retptr = ret)
            {
                Memory.Copy(CellPtr, retptr, length);
                return ret;
            }
        }
        /// <summary>
        /// Performs the specified action on each elements
        /// </summary>
        /// <param name="action">A lambda expression which has one parameter indicates element in List</param>
        public unsafe void ForEach(Action<)::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::(> action)
        {
            byte* targetPtr = CellPtr;
            byte* endPtr = CellPtr + length;
            while (targetPtr < endPtr)
            {
                )::");
if (!element_need_accessor_1)
{
source->append(R"::(
                {
                    action(*()::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::(*)targetPtr);
                    targetPtr += )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::(;
                }
                )::");
}
else if (element_fixed_1)
{
source->append(R"::(
                {
                    elementAccessor.CellPtr = targetPtr;
                    action(elementAccessor);
                    targetPtr += )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::(;
                }
                )::");
}
else if (node->listElementType->is_struct())
{
source->append(R"::(
                {
                    elementAccessor.CellPtr = targetPtr;
                    action(elementAccessor);
                }
                )::");
}
else
{
source->append(R"::(
                {
                    elementAccessor.CellPtr = targetPtr + 4;
                    action(elementAccessor);
                }
                )::");
}
source->append(R"::(
            }
        }
        /// <summary>
        /// Performs the specified action on each elements
        /// </summary>
        /// <param name="action">A lambda expression which has two parameters. First indicates element in the List and second the index of this element.</param>
        public unsafe void ForEach(Action<)::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::(, int> action)
        {
            byte* targetPtr = CellPtr;
            byte* endPtr = CellPtr + length;
            for (int index = 0; targetPtr < endPtr; ++index)
            {
                )::");
if (!element_need_accessor_1)
{
source->append(R"::(
                {
                    action(*()::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::(*)targetPtr, index);
                    targetPtr += )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::(;
                }
                )::");
}
else if (element_fixed_1)
{
source->append(R"::(
                {
                    elementAccessor.CellPtr = targetPtr;
                    action(elementAccessor, index);
                    targetPtr += )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::(;
                }
                )::");
}
else if (node->listElementType->is_struct())
{
source->append(R"::(
                {
                    elementAccessor.CellPtr = targetPtr;
                    action(elementAccessor, index);
                }
                )::");
}
else
{
source->append(R"::(
                {
                    elementAccessor.CellPtr = targetPtr + 4;
                    action(elementAccessor, index);
                }
                )::");
}
source->append(R"::(
            }
        }
        internal unsafe struct _iterator
        {
            byte* targetPtr;
            byte* endPtr;
            )::");
source->append(Codegen::GetString(data_type_get_accessor_name(node)));
source->append(R"::( target;
            internal _iterator()::");
source->append(Codegen::GetString(data_type_get_accessor_name(node)));
source->append(R"::( target)
            {
                targetPtr = target.CellPtr;
                endPtr = targetPtr + target.length;
                this.target = target;
            }
            internal bool good()
            {
                return (targetPtr < endPtr);
            }
            internal )::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::( current()
            {
                )::");
if (!element_need_accessor_1)
{
source->append(R"::(
                {
                    return *()::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::(*)targetPtr;
                }
                )::");
}
else if (element_fixed_1)
{
source->append(R"::(
                {
                    target.elementAccessor.CellPtr = targetPtr;
                    return (target.elementAccessor);
                }
                )::");
}
else if (node->listElementType->is_struct())
{
source->append(R"::(
                {
                    target.elementAccessor.CellPtr = targetPtr;
                    return target.elementAccessor;
                }
                )::");
}
else
{
source->append(R"::(
                {
                    target.elementAccessor.CellPtr = targetPtr + 4;
                    return target.elementAccessor;
                }
                )::");
}
source->append(R"::(
            }
            internal void move_next()
            {
                )::");
if (!element_need_accessor_1)
{
source->append(R"::(
                {
                    targetPtr += )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::(;
                }
                )::");
}
else if (element_fixed_1)
{
source->append(R"::(
                {
                    targetPtr += )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::(;
                }
                )::");
}
else if (node->listElementType->is_struct())
{
source->append(R"::(
                {
                }
                )::");
}
else
{
source->append(R"::(
                {
                }
                )::");
}
source->append(R"::(
            }
        }
        public IEnumerator<)::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::(> GetEnumerator()
        {
            _iterator _it = new _iterator(this);
            while (_it.good())
            {
                yield return _it.current();
                _it.move_next();
            }
        }
        unsafe IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
        /// <summary>
        /// Adds an item to the end of the List
        /// </summary>
        /// <param name="element">The object to be added to the end of the List.</param>
        public unsafe void Add()::");
source->append(Codegen::GetString(node->listElementType));
source->append(R"::( element)
        {
            byte* targetPtr = null;
            {
            }
        }
        /// <summary>
        /// Inserts an element into the List at the specified index.
        /// </summary>
        /// <param name="index">The zero-based index at which item should be inserted.</param>
        /// <param name="element">The object to insert.</param>
        public unsafe void Insert(int index, )::");
source->append(Codegen::GetString(node->listElementType));
source->append(R"::( element)
        {
            if (index < 0 || index > Count) throw new IndexOutOfRangeException();
            byte* targetPtr = null;
            {
            }
            int size = (int)targetPtr;
            targetPtr = CellPtr;
            )::");
if (element_fixed_1)
{
source->append(R"::(
            targetPtr += index * )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::(;
            )::");
}
else
{
source->append(R"::(
            for (int i = 0; i < index; i++)
            {
            }
            )::");
}
source->append(R"::(
            int offset = (int)(targetPtr - CellPtr);
            targetPtr = this.CellPtr + offset;
        }
        /// <summary>
        /// Inserts an element into a sorted list.
        /// </summary>
        /// <param name="element">The object to insert.</param>
        /// <param name="comparison"></param>
        public unsafe void Insert()::");
source->append(Codegen::GetString(node->listElementType));
source->append(R"::( element, Comparison<)::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::(> comparison)
        {
            byte* targetPtr = null;
            {
            }
            int size = (int)targetPtr;
            targetPtr = CellPtr;
            byte* endPtr = CellPtr + length;
            while (targetPtr<endPtr)
            {
                )::");
if (!element_need_accessor_1)
{
source->append(R"::(
                {
                    if (comparison(*()::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::(*)targetPtr, element) <= 0)
                    {
                        targetPtr += )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::(;
                    }
                    else
                    {
                        break;
                    }
                }
                )::");
}
else if (element_fixed_1)
{
source->append(R"::(
                {
                    elementAccessor.CellPtr = targetPtr;
                    if (comparison(elementAccessor, element)<=0)
                    {
                        targetPtr += )::");
source->append(Codegen::GetString(element_len_1));
source->append(R"::(;
                    }
                    else
                    {
                        break;
                    }
                }
                )::");
}
else if (node->listElementType->is_struct())
{
source->append(R"::(
                {
                    elementAccessor.CellPtr = targetPtr;
                    if (comparison(elementAccessor, element)<=0)
                    {
                    }
                    else
                    {
                        break;
                    }
                }
                )::");
}
else
{
source->append(R"::(
                {
                    elementAccessor.CellPtr = targetPtr + 4;
                    if (comparison(elementAccessor, element)<=0)
                    {
                    }
                    else
                    {
                        break;
                    }
                }
                )::");
}
source->append(R"::(
            }
            int offset = (int)(targetPtr - CellPtr);
            targetPtr = this.CellPtr + offset;
        }
        /// <summary>
        /// Removes the element at the specified index of the List.
        /// </summary>
        /// <param name="index">The zero-based index of the element to remove.</param>
        public unsafe void RemoveAt(int index)
        {
            if (index < 0 || index >= Count) throw new IndexOutOfRangeException();
            byte* targetPtr = CellPtr;
            for (int i = 0; i < index; i++)
            {
            }
            int offset = (int)(targetPtr - CellPtr);
            byte* oldtargetPtr = targetPtr;
            int size = (int)(oldtargetPtr - targetPtr);
        }
        /// <summary>
        /// Adds the elements of the specified collection to the end of the List
        /// </summary>
        /// <param name="collection">The collection whose elements should be added to the end of the List. The collection itself cannot )::");
source->append(R"::(be null.</param>
        public unsafe void AddRange()::");
source->append(Codegen::GetString(node));
source->append(R"::( collection)
        {
            if (collection == null) throw new ArgumentNullException("collection is null.");
            )::");
source->append(Codegen::GetString(data_type_get_accessor_name(node)));
source->append(R"::( tcollection = collection;
            int delta = tcollection.length;
            CellPtr = ResizeFunction(CellPtr - 4, *(int*)(CellPtr - 4) + 4, delta);
            Memory.Copy(tcollection.CellPtr, CellPtr + *(int*)CellPtr + 4, delta);
            *(int*)CellPtr += delta;
            this.CellPtr += 4;
        }
        /// <summary>
        /// Adds the elements of the specified collection to the end of the List
        /// </summary>
        /// <param name="collection">The collection whose elements should be added to the end of the List. The collection itself cannot be null.</param>
        public unsafe void AddRange()::");
source->append(Codegen::GetString(data_type_get_accessor_name(node)));
source->append(R"::( collection)
        {
            if (collection == null) throw new ArgumentNullException("collection is null.");
            int delta = collection.length;
            if (collection.CellID != CellID)
            {
                CellPtr = ResizeFunction(CellPtr - 4, *(int*)(CellPtr - 4) + 4, delta);
                Memory.Copy(collection.CellPtr, CellPtr + *(int*)CellPtr + 4, delta);
                *(int*)CellPtr += delta;
            }
            else
            {
                byte[] tmpcell = new byte[delta];
                fixed (byte* tmpcellptr = tmpcell)
                {
                    Memory.Copy(collection.CellPtr, tmpcellptr, delta);
                    CellPtr = ResizeFunction(CellPtr - 4, *(int*)(CellPtr - 4) + 4, delta);
                    Memory.Copy(tmpcellptr, CellPtr + *(int*)CellPtr + 4, delta);
                    *(int*)CellPtr += delta;
                }
            }
            this.CellPtr += 4;
        }
        /// <summary>
        /// Remove)::");
source->append(R"::(s all elements from the List
        /// </summary>
        public unsafe void Clear()
        {
            int delta = length;
            Memory.memset(CellPtr, 0, (ulong)delta);
            CellPtr = ResizeFunction(CellPtr - 4, 4, -delta);
            *(int*)CellPtr = 0;
            this.CellPtr += 4;
        }
        /// <summary>
        /// Determines whether an element is in the List
        /// </summary>
        /// <param name="item">The object to locate in the List.The value can be null for non-atom types</param>
        /// <returns>true if item is found in the List; otherwise, false.</returns>
        public unsafe bool Contains()::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::( item)
        {
            bool ret = false;
            ForEach(x =>
            {
                if (item == x) ret = true;
            });
            return ret;
        }
        /// <summary>
        /// Determines whether the List contains elements that match the conditions defined by the specified predicate.
        /// </summary>
        /// <param name="match">The Predicate delegate that defines the conditions of the elements to search for.</param>
        /// <returns>true if the List contains one or more elements that match the conditions defined by the specified predicate; otherwise, false.</returns>
        public unsafe bool Exists(Predicate<)::");
source->append(Codegen::GetString(data_type_get_accessor_name(node->listElementType)));
source->append(R"::(> match)
        {
            bool ret = false;
            ForEach(x =>
            {
                if (match(x)) ret = true;
            });
            return ret;
        }
        /// <summary>
        /// Copies the entire List to a compatible one-dimensional array, starting at the beginning of the ptr1 array.
        /// </summary>
        /// <param name="array">The one-dimensional Array that is the destination of the elements copied from List. The Array must have zero-based indexing.</param>
        public unsafe void CopyTo()::");
source->append(Codegen::GetString(node->listElementType));
source->append(R"::([] array)
        {
            if (array == null) throw new ArgumentNullException("array is null.");
            if (array.Length < Count) throw new ArgumentException("The number of elements in the source List is greater than the number of elements that the destination array can contain.");
            ForEach((x, i) => array[i] = x);
        }
        /// <summary>
        /// Copies the entire List to a compatible one-dimensional array, starting at the specified index of the ptr1 array.
        /// </summary>
        /// <param name="array">The one-dimensional Array that is the destination of the elements copied from List. The Array must have zero-based indexing.</param>
        /// <param name="arrayIndex">The zero-based index in array at which copying begins.</param>
        public unsafe void CopyTo()::");
source->append(Codegen::GetString(node->listElementType));
source->append(R"::([] array, int arrayIndex)
        {
            if (array == null) throw new ArgumentNullException("array is null.");
            if (arrayIndex < 0) throw new ArgumentOutOfRangeException("arrayIndex is less than 0.");
            if (array.Length - arrayIndex < Count) throw new ArgumentException("The number of elements in the source List is greater than the available space from arrayIndex to the end of the destination array.");
            ForEach((x, i) => array[i + arrayIndex] = x);
        }
        /// <summary>
        /// Copies a range of elements from the List to a compatible one-dimensional array, starting at the specified index of the ptr1 array.
        /// </summary>
        /// <param name="index">The zero-based index in the source List at which copying begins.</param>
        /// <param name="array">The one-dimensional Array that is the destination of the elements copied from List. The Array must have zero-based indexing.</param>
        /// <param name="arrayIndex">The zero-based i)::");
source->append(R"::(ndex in array at which copying begins.</param>;
        /// <param name="count">The number of elements to copy.</param>
        public unsafe void CopyTo(int index, )::");
source->append(Codegen::GetString(node->listElementType));
source->append(R"::([] array, int arrayIndex, int count)
        {
            if (array == null) throw new ArgumentNullException("array is null.");
            if (arrayIndex < 0 || index < 0 || count < 0) throw new ArgumentOutOfRangeException("arrayIndex is less than 0 or index is less than 0 or count is less than 0.");
            if (array.Length - arrayIndex < Count - index) throw new ArgumentException("The number of elements from index to the end of the source List is greater than the available space from arrayIndex to the end of the destination array. ");
            if (index >= Count) throw new ArgumentException("index is equal to or greater than the Count of the source List.");
            int j = 0;
            for (int i = index; i < index + count; i++)
            {
                array[j + arrayIndex] = this[i];
                ++j;
            }
        }
        /// <summary>
        /// Inserts the elements of a collection into the List at the specified index.
        /// </summary>
        /// )::");
source->append(R"::(<param name="index">The zero-based index at which the new elements should be inserted.</param>
        /// <param name="collection">The collection whose elements should be inserted into the List. The collection itself cannot be null, but it can contain elements that are null, if type T is a reference type.</param>
        public unsafe void InsertRange(int index, )::");
source->append(Codegen::GetString(node));
source->append(R"::( collection)
        {
            if (collection == null) throw new ArgumentNullException("collection is null.");
            if (index < 0) throw new ArgumentOutOfRangeException("index is less than 0.");
            if (index > Count) throw new ArgumentOutOfRangeException("index is greater than Count.");
            )::");
source->append(Codegen::GetString(data_type_get_accessor_name(node)));
source->append(R"::( tmpAccessor = collection;
            byte* targetPtr = CellPtr;
            for (int i = 0; i < index; i++)
            {
            }
            int offset = (int)(targetPtr - CellPtr);
            CellPtr = ResizeFunction(CellPtr - 4, offset + 4, tmpAccessor.length);
            Memory.Copy(tmpAccessor.CellPtr, CellPtr + offset + 4, tmpAccessor.length);
            *(int*)CellPtr += tmpAccessor.length;
            this.CellPtr += 4;
        }
        /// <summary>
        /// Removes a range of elements from the List.
        /// </summary>
        /// <param name="index">The zero-based starting index of the range of elements to remove.</param>
        /// <param name="count">The number of elements to remove.</param>
        public unsafe void RemoveRange(int index, int count)
        {
            if (index < 0) throw new ArgumentOutOfRangeException("index is less than 0.");
            if (index > Count) throw new ArgumentOutOfRangeException("index is greater than Count.");
       )::");
source->append(R"::(     if (index + count > Count) throw new ArgumentException("index and count do not denote a valid range of elements in the List.");
            byte* targetPtr = CellPtr;
            for (int i = 0; i < index; i++)
            {
            }
            int offset = (int)(targetPtr - CellPtr);
            byte* oldtargetPtr = targetPtr;
            for (int i = 0; i < count; i++)
            {
            }
            int size = (int)(oldtargetPtr - targetPtr);
            CellPtr = ResizeFunction(CellPtr - 4, offset + 4, size);
            *(int*)CellPtr += size;
            this.CellPtr += 4;
        }
        public static bool operator ==()::");
source->append(Codegen::GetString(data_type_get_accessor_name(node)));
source->append(R"::( a, )::");
source->append(Codegen::GetString(data_type_get_accessor_name(node)));
source->append(R"::( b)
        {
            if (ReferenceEquals(a, b))
                return true;
            if (ReferenceEquals(a, null) || ReferenceEquals(b, null))
                return false;
            if (a.CellPtr == b.CellPtr) return true;
            if (a.length != b.length) return false;
            return Memory.Compare(a.CellPtr, b.CellPtr, a.length);
        }
        public static bool operator !=()::");
source->append(Codegen::GetString(data_type_get_accessor_name(node)));
source->append(R"::( a, )::");
source->append(Codegen::GetString(data_type_get_accessor_name(node)));
source->append(R"::( b)
        {
            return !(a == b);
        }
    }
}
)::");

            return source;
        }
    }
}
