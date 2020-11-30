; ModuleID = 'multithread.c'
source_filename = "multithread.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@constGlobal = dso_local constant i32 10, align 4
@constChar = dso_local constant [8 x i8] c"content\00", align 1
@Global = common dso_local global i32 0, align 4
@FuncUsingStaticConst.sta = internal constant i32 0, align 4
@FuncUsingStaticVariable.sta = internal global i32 0, align 4
@FuncReturnStaticConst.sta = internal constant [11 x i8] c"stacontent\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @Thread1(i8*) #0 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = load i32, i32* @Global, align 4
  %4 = add nsw i32 %3, 1
  store i32 %4, i32* @Global, align 4
  ret i8* null
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @Thread2(i8*) #0 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = load i32, i32* @Global, align 4
  %4 = add nsw i32 %3, -1
  store i32 %4, i32* @Global, align 4
  ret i8* null
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @FuncUsingGlobalConst(i8*) #0 {
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  %4 = alloca i8, align 1
  store i8* %0, i8** %2, align 8
  store i32 10, i32* %3, align 4
  %5 = load i8, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @constChar, i64 0, i64 0), align 1
  store i8 %5, i8* %4, align 1
  ret i8* null
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @FuncUsingStaticConst(i8*) #0 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  ret i8* null
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @FuncUsingGlobalVariable(i8*) #0 {
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  store i8* %0, i8** %2, align 8
  %4 = load i32, i32* @Global, align 4
  store i32 %4, i32* %3, align 4
  %5 = load i32, i32* %3, align 4
  ret i32 %5
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @FuncUsingStaticVariable(i8*) #0 {
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  store i8* %0, i8** %2, align 8
  %4 = load i32, i32* @FuncUsingStaticVariable.sta, align 4
  store i32 %4, i32* %3, align 4
  %5 = load i32, i32* %3, align 4
  ret i32 %5
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @FuncUsingNewSpace(i8*) #0 {
  %2 = alloca i8*, align 8
  %3 = alloca i32*, align 8
  store i8* %0, i8** %2, align 8
  %4 = call noalias i8* @malloc(i64 4) #3
  %5 = bitcast i8* %4 to i32*
  store i32* %5, i32** %3, align 8
  ret i8* null
}

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @FuncCallNonReentrantFunc(i8*) #0 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = call i8* @Thread1(i8* null)
  ret i8* null
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @FuncCallReentrantFunc(i8*) #0 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  %3 = call i8* @FuncUsingGlobalConst(i8* null)
  ret i8* null
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @FuncReturnGlobalConst(i8*) #0 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  ret i8* getelementptr inbounds ([8 x i8], [8 x i8]* @constChar, i64 0, i64 0)
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @FuncReturnStaticConst(i8*) #0 {
  %2 = alloca i8*, align 8
  store i8* %0, i8** %2, align 8
  ret i8* getelementptr inbounds ([11 x i8], [11 x i8]* @FuncReturnStaticConst.sta, i64 0, i64 0)
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca [2 x i64], align 16
  %2 = getelementptr inbounds [2 x i64], [2 x i64]* %1, i64 0, i64 0
  %3 = call i32 @pthread_create(i64* %2, %union.pthread_attr_t* null, i8* (i8*)* @Thread1, i8* null) #3
  %4 = getelementptr inbounds [2 x i64], [2 x i64]* %1, i64 0, i64 1
  %5 = call i32 @pthread_create(i64* %4, %union.pthread_attr_t* null, i8* (i8*)* @Thread2, i8* null) #3
  %6 = getelementptr inbounds [2 x i64], [2 x i64]* %1, i64 0, i64 0
  %7 = load i64, i64* %6, align 16
  %8 = call i32 @pthread_join(i64 %7, i8** null)
  %9 = getelementptr inbounds [2 x i64], [2 x i64]* %1, i64 0, i64 1
  %10 = load i64, i64* %9, align 8
  %11 = call i32 @pthread_join(i64 %10, i8** null)
  ret i32 0
}

; Function Attrs: nounwind
declare !callback !2 dso_local i32 @pthread_create(i64*, %union.pthread_attr_t*, i8* (i8*)*, i8*) #1

declare dso_local i32 @pthread_join(i64, i8**) #2

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 9.0.0-2 (tags/RELEASE_900/final)"}
!2 = !{!3}
!3 = !{i64 2, i64 3, i1 false}
