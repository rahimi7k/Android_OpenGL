# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.
#
# For more details, see
# http://developer.android.com/guide/developing/tools/proguard.html

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

# Uncomment this to preserve the line number information for
# debugging stack traces.
#-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
#-renamesourcefileattribute SourceFile

# don't change .** to .* not work

-keep class java.** {*;}
#-dontwarn java.**

-keep class javax.** {*;}
#-dontwarn javax.**

-keep class androidx.** {*;}
-keep interface androidx.* {*;}
#-dontwarn androidx.**
-keep class com.android.** {*;}
-keep class android.support.** {*;}

-keep class org.sqlite.** {*;}
-keep class com.sqlite.** {*;}

-keep class com.example.game.** {*;}
-keep class com.example.game.Native$** {*;}
#-keepclassmembers class io.dynamic.follower.ionet.IOApi$** {*;}

-assumenosideeffects class com.example.game.Log {
	public static *** v(...);
	public static *** i(...);
	public static *** w(...);
	public static *** d(...);
	public static *** e(...);
}

-verbose
#-dalvik

# don't use this!
#-dontobfuscate

# not use code remove from source
-dontshrink

# Specifies not to verify the processed class files.
#-dontpreverify

# https://stackoverflow.com/questions/35321742/android-proguard-most-aggressive-optimizations
-dontoptimize
#-optimizations !code/simplification/arithmetic,!code/simplification/cast,!field/*,!class/merging/*
#-optimizations !code/simplification/cast,!code/simplification/advanced,!field/*,!class/merging/*,!method/removal/parameter,!method/propagation/parameter
#-optimizationpasses 5

# https://stackoverflow.com/questions/57156648/how-to-add-code-obfuscation-for-my-android-application
# Specifies not to ignore non-public library classes.
-dontskipnonpubliclibraryclasses

# Specifies not to ignore package visible library class members
-dontskipnonpubliclibraryclassmembers

# Specifies that the access modifiers of classes and class members may have become broad during processing. This can improve the results of the optimization step.
-allowaccessmodification

# Specifies that interfaces may be merged, even if their implementing classes don't implement all interface methods. This can reduce the size of the output by reducing the total number of classes.
-mergeinterfacesaggressively

# Specifies to apply aggressive overloading while obfuscating. Multiple fields and methods can then get the same names, This option can make the processed code even smaller
-overloadaggressively

# Specifies to repackage all packages that are renamed, by moving them into the single given parent package
#-flattenpackagehierarchy

# Specifies to repackage all class files that are renamed, by moving them into the single given package. Without argument or with an empty string (''), the package is removed completely.
#-repackageclasses ''

# For example, if your code contains a large number of hard-coded strings that refer to classes, and you prefer not to keep their names, you may want to use this option
-adaptclassstrings

# Specifies the resource files to be renamed, all resource files that correspond to class files are renamed
-adaptresourcefilenames

# Specifies the resource files whose contents are to be updated. Any class names mentioned in the resource files are renamed
-adaptresourcefilecontents

#-dontnote androidx.annotation.Keep
-keepattributes *Annotation*

-keep class androidx.annotation.Keep
-keep @androidx.annotation.Keep class * {*;}

-keepclasseswithmembers class * {
	@androidx.annotation.Keep <methods>;
}

-keepclasseswithmembers class * {
	@androidx.annotation.Keep <fields>;
}

-keepclasseswithmembers class * {
	@androidx.annotation.Keep <init>(...);
}

-keep class android.support.annotation.Keep

-keep @android.support.annotation.Keep class * {*;}

-keepclasseswithmembers class * {
	@android.support.annotation.Keep <methods>;
}

-keepclasseswithmembers class * {
	@android.support.annotation.Keep <fields>;
}

-keepclasseswithmembers class * {
	@android.support.annotation.Keep <init>(...);
}

-keep public class * extends android.app.Activity

-keepclassmembers class * extends android.app.Activity {
	public void *(android.view.View);
}

# keep setters in Views so that animations can still work.
# see http://proguard.sourceforge.net/manual/examples.html#beans
-keepclassmembers public class * extends android.view.View {
	public <init>(android.content.Context);
	public <init>(android.content.Context, android.util.AttributeSet);
	public <init>(android.content.Context, android.util.AttributeSet, int);
	public <init>(android.content.Context, android.util.AttributeSet, int, int);
	public void set*(***);
	*** get*();
}

-keepclassmembers class * implements android.os.Parcelable {
	public static final android.os.Parcelable$Creator CREATOR;
}

-keepclassmembers class **.R$* {
	public static <fields>;
}

# For native methods, see http://proguard.sourceforge.net/manual/examples.html#native
-keepclasseswithmembernames, includedescriptorclasses class * {
	native <methods>;
}

# For enumeration classes, see http://proguard.sourceforge.net/manual/examples.html#enumerations
-keepclassmembers enum * {
	public static **[] values();
	public static ** valueOf(java.lang.String);
}

# Remove Android logging code (at least in the release version, for which
# optimization is switched on). This is important, to remove many obvious
# debug traces in the License Verification Library.
# For enumeration classes, see http://proguard.sourceforge.net/manual/examples.html#enumerations
-assumenosideeffects class android.util.Log {
	public static boolean isLoggable(java.lang.String, int);
	public static *** v(...);
	public static *** i(...);
	public static *** w(...);
	public static *** d(...);
	public static *** e(...);
	# Warning: removing terrible failure logging calls might change behavior
	# for devices < API Level 23.
	public static *** wtf(...);
}
