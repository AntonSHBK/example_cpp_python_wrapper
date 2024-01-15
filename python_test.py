import CppToPython

print(CppToPython.add(40, 10))
print(CppToPython.bob(40, 10))

print(CppToPython.sum_thread_ids())
print(CppToPython.get_max_threads())

this_talker = CppToPython.SomeTalker('Hello world!')

print(this_talker.get_omp_max_treads())
this_talker.get_text_parallel(4)
