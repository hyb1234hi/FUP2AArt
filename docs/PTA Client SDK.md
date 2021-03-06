# P2A Client SDK-iOS

本文主要介绍了如何快速跑通我们的FUP2A工程 、如何创建和编辑风格化形象、如何绘制风格化形象、SDK的分类及相关资源说明等。工程中会使用到两个库文件:FUP2AClient SDK、Nama SDK，其中 FUP2AClient SDK 用来做风格化形象的生成，Nama SDK 用来做风格化形象的绘制。

## 快速开始

下载工程后需要先获取两个证书：

* authpack.h：Nama SDK鉴权证书，用于在客户端，使用Nama SDK 绘制的鉴权。


下载证书后，将证书拷贝到[Faceunity](FUP2A/Faceunity)文件夹下，直接运行工程即可。

## 资源说明

### SDK

- **libp2a_client.a**： FUP2AClient SDK 负责头和头发的创建，以及头部编辑的功能。不需要鉴权即可使用。
- **libnama.a**: Nama SDK，进行风格化形象绘制，需要有鉴权证书才能使用。Nama SDK的接口与资源详细说明，请查看[FULiveDemo 说明文档](https://github.com/Faceunity/FULiveDemo)。

### 道具

- **controller.bundle**：风格化形象的控制中心，负责绑定头、身体、衣服、胡子、头发、AR滤镜、眼镜、帽子等配饰。并负责捏脸、发色修改、胡子颜色修改、肤色修改、唇色修改、配饰颜色修改、缩放、旋转、身体动画、AR模式、人脸跟踪、表情裁剪等诸多功能的控制。实际绘制时只需要将controller道具的句柄传入Nama的render接口进行绘制即可，关于controller参数使用方法，请查看[controller说明文档](Controller%20%E8%AF%B4%E6%98%8E%E6%96%87%E6%A1%A3.md)。
- **controller_config.bundle**：controller配置文件，每创建一个controller必须绑定controller_config。
- **head.bundle**：头道具，不同的人生成的头不一样，需要绑定到controller道具上才能使用。
- **body.bundle**：身体道具，男女各一个身体，需要绑定到controller道具上才能使用。 
- **hair.bundle**：预置头发道具，用于形象生成时生成与头道具大小相匹配的头发道具。可以修改发色，新生成的头发道具需要绑定到controller道具上才能使用。
- **beard.bundle**：胡子道具，有多种款式，可以修改胡子颜色，需要绑定到controller道具上才能使用。
- **upper.bundle**：衣服道具，套装，同时包含了上衣和下衣，有多种款式分男女，根据不同类型的衣服加载不同性别的身体，需要绑定到controller道具上才能使用，无法与上衣和下衣同时使用。
- **upper.bundle**：上衣道具，有多种款式分男女，根据不同类型的衣服加载不同性别的身体，配合下衣一起使用，需要绑定到controller道具上才能使用。
- **lower.bundle**：下衣道具，有多种款式分男女，根据不同类型的衣服加载不同性别的身体，配合上衣一起使用，需要绑定到controller道具上才能使用。
- **eyebrow.bundle**：眉毛道具，有多种款式，需要绑定到controller道具上才能使用。
- **eyelash.bundle**：睫毛道具，有多种款式，需要绑定到controller道具上才能使用。
- **glass.bundle**：眼镜道具，有多种款式，可以修改镜框及镜片颜色，需要绑定到controller道具上才能使用。
- **shoes.bundle**：鞋子道具，有多种款式，需要绑定到controller道具上才能使用。
- **animation.bundle**：动画道具，有多种动画类型，需要绑定到controller道具上才能使用。
- **default_bg.bundle**：背景道具，有多种背景道具，需绑定到controller道具上才能使用。

## 功能简介

本工程主要包括以下功能：

* 形象生成：上传照片到服务端对人脸进行检测，利用服务端返回的数据生成风格化形象；对风格化形象进行美型后，重新生成形象。
* 形象绘制：实现风格化形象的实时绘制。
* 形象驱动：通过人脸驱动风格化形象。
* 形象编辑：支持美型，以及对肤色、唇色、瞳色、发型、胡子、眼镜、帽子、衣服、上衣、下衣、配饰的个性化编辑；
* 文字驱动：输入文字（200个字符以内），实时驱动风格化形象说话。
* Ar驱动：Ar环境下，通过人脸，实时驱动风格化形象。
* 形象应用：支持单人场景、多人场景、动画场景的合影和 GIF动图的导出；

## 形象生成

首先上传照片到服务端做人脸检测，并得到服务端返回的数据，然后使用服务端返回的数据调用FUP2AClient SDK来创建头和头发道具。另外当对风格化形象进行美型后，也需要重新生成形象的头道具。主要流程如下：

* 上传照片
* 初始化 FUP2AClient SDK 
* 使用 FUP2AClient SDK 生成头道具
* 使用 FUP2AClient SDK 生成头发道具
* 使用 FUP2AClient SDK 重新生成头道具

### 上传照片

用户上传照片到服务端，服务端对该图片做人脸检测，并返回检测后的人脸数据： head.bundle。head.bundle 包含用户的发型、肤色、眼镜、唇色、脸型等详细信息。

### 初始化 FUP2AClient SDK 

调用 FUP2AClient SDK 相关接口前，需要先进行初始化，且只需要初始化一次。

初始化接口说明如下：

```objective-c

/**
*  初始化 FUP2AClient data
- 需要先初始化 data 才能使用其他接口，全局只需要初始化 data 一次

*    @param coreDataPath        p2a_client_core 文件路径
*    @param package            密钥数组，必须配置好密钥，SDK 才能正常工作
*    @param size                密钥数组大小
*/
-(void)setupCore:(NSString *)coreDataPath
    authPackage : (void *)package
    authSize : (int)size;
```

根据形象风格设置customData，切换风格时需要再次设置对应数据
```objective-c
/**
*    切换 FUP2AClient data

*    @param customDataPath  customData 文件路径
*/
-(void)setupCustomData:(NSString *)customDataPath;
```

示例代码
```objective-c
- (void)loadClientDataWithFirstSetup:(BOOL)firstSetup {
    
    NSString *qPath ;
    switch (self.avatarStyle) {
        case FUAvatarStyleNormal:{
            if (![[NSFileManager defaultManager] fileExistsAtPath:AvatarListPath]) {
                [[NSFileManager defaultManager] createDirectoryAtPath:AvatarListPath withIntermediateDirectories:YES attributes:nil error:nil];
            }
            qPath =[[NSBundle mainBundle] pathForResource:@"p2a_client_q" ofType:@"bin"];
        }
            break;
        case FUAvatarStyleQ:{
            if (![[NSFileManager defaultManager] fileExistsAtPath:AvatarQPath]) {
                [[NSFileManager defaultManager] createDirectoryAtPath:AvatarQPath withIntermediateDirectories:YES attributes:nil error:nil];
            }
            qPath =[[NSBundle mainBundle] pathForResource:@"p2a_client_q1" ofType:@"bin"];
        }
            break ;
    }
    // p2a bin
    if (firstSetup) {
        NSString *corePath = [[NSBundle mainBundle] pathForResource:@"p2a_client_core" ofType:@"bin"];
        [[fuPTAClient shareInstance] setupCore:corePath authPackage:&g_auth_package authSize:sizeof(g_auth_package)];
    }
    [[fuPTAClient shareInstance] setupCustomData:qPath];
}

```

### 生成头道具

向我司服务器上传一张人像图片，服务器会返回一个NSData对象，将NSData写入本地，并保存名为head.bundle的文件，head.bundle即为新生成的头道具：



示例代码
```objective-c
- (FUAvatar *)createAvatarWithData:(NSData *)data avatarName:(NSString *)name gender:(FUGender)gender 
{
    isCreatingAvatar = YES ;

    FUAvatar *avatar = [[FUAvatar alloc] init];
    avatar.defaultModel = NO ;
    avatar.name = name;
    avatar.gender = gender ;
    BOOL isQ = self.avatarStyle == FUAvatarStyleQ ;
    avatar.isQType = isQ ;

    //生成头
    [data writeToFile:[[avatar filePath] stringByAppendingPathComponent:FU_HEAD_BUNDLE] atomically:YES];
    [[fuPTAClient shareInstance] setHeadData:data];
  
  //其他代码
}
```
若要获取head.bundle相关的属性，先要调用[fuPTAClient shareInstance] setHeadData:data]方法，例如，如果需要获取当前上传图片中人像匹配的眼镜，使用以下方法：
```objective-c
	[[fuPTAClient shareInstance] setHeadData:data];
	// 眼镜
	int hasGlass = [[fuPTAClient shareInstance] getInt:@"has_glasses"];
```


注：该接口支持异步并行调用。

### 生成头发道具

使用 head.bundle 与预置的 hair.bundle，调用 FUP2AClient SDK 相关接口，生成与头道具大小相匹配的头发道具，相关API接口说明如下：

```objective-c
/**
*  生成 hair.Bundle
- 根据服务端传回的数据流和预置的头发模型 生成和此头部模型匹配的头发模型

*  @param headData         head.bundle
*  @param hairData        预置头发模型数据

*  @return                生成的头发模型数据
*/
-(NSData *)createHairWithHeadData:(NSData *)headData
    defaultHairData : (NSData *)hairData;
```

示例代码
```objective-c
- (FUAvatar *)createAvatarWithData:(NSData *)data avatarName:(NSString *)name gender:(FUGender)gender 
{
    isCreatingAvatar = YES ;

    FUAvatar *avatar = [[FUAvatar alloc] init];
    avatar.defaultModel = NO ;
    avatar.name = name;
    avatar.gender = gender ;
    BOOL isQ = self.avatarStyle == FUAvatarStyleQ ;
    avatar.isQType = isQ ;

    //生成头
    [data writeToFile:[[avatar filePath] stringByAppendingPathComponent:FU_HEAD_BUNDLE] atomically:YES];
    [[fuPTAClient shareInstance] setHeadData:data];
    
    //生成头发
    int hairLabel = [[fuPTAClient shareInstance] getInt:@"hair_label"];
    avatar.hairLabel = hairLabel ;
    NSString *defaultHair = [self gethairNameWithNum:hairLabel andGender:gender];
    avatar.hair = defaultHair ;
    
    NSString *baseHairPath = [[NSBundle mainBundle] pathForResource:avatar.hair ofType:nil] ;
    NSData *baseHairData = [NSData dataWithContentsOfFile: baseHairPath];
    NSData *defaultHairData = [[fuPTAClient shareInstance] createHairWithHeadData:data defaultHairData:baseHairData];
    NSString *defaultHairPath = [[avatar filePath] stringByAppendingPathComponent:defaultHair];
    [defaultHairData writeToFile:defaultHairPath atomically:YES];
    [[fuPTAClient shareInstance] setHeadData:data];
  
  //其他代码
}
```

注：该接口支持异步并行调用。

### 重新生成头道具

对风格化形象进行美型后，重新生成形象的头道具。需要调用 FUP2AClient SDK 的 deformAvatarHeadWithHeadData 接口生成新的头道具，API接口说明如下：

API接口说明如下：

```objective-c
/**
*  对已现有头部模型进行编辑
- 对现有的头部模型进行形变处理，生成一个新的头部模型

*  @param headData            现有的头部模型数据, head.bundle
*  @param deformParams        形变参数
*  @param paramsSize        形变参数大小
*  @param withExprOnly        是否生成只包含expression的bundle
*  @param withLowp            是否生成低精度的bundle

*  @return                    新的头部模型数据
*/
-(fuPTAHeadBundle *)deformHeadWithHeadData:(NSData *)headData
    deformParams : (float *)deformParams
    paramsSize : (NSInteger)paramsSize
    withExprOnly : (BOOL)withExprOnly
    withLowp : (BOOL)withLowp;
```


示例代码
```objective-c
/**
 捏脸之后生成新的 Avatar
 
 @param coeffi  捏脸参数
 @param deform  是否 deform
 @return        新的 Avatar
 */
- (FUAvatar *)createPupAvatarWithCoeffi:(float *)coeffi DeformHead:(BOOL)deform {
    
    FUAvatar *currentAvatar = self.currentAvatars.firstObject ;
    
    NSData *headData = [NSData dataWithContentsOfFile:[[currentAvatar filePath] stringByAppendingPathComponent:FU_HEAD_BUNDLE]];
    
    if (deform) {
        headData = [[fuPTAClient shareInstance] deformHeadWithHeadData:headData deformParams:coeffi paramsSize:90 withExprOnly:NO withLowp:NO].bundle;
    }
    
    //其他代码
}
```

注：该接口支持异步并行调用。

## 形象绘制

使用 FUP2AClient SDK 生成的风格化形象，目前支持通过 Nama SDK 进行绘制，后续将支持使用其他绘制引擎进行绘制，如 Unity 3D。使用 Nama SDK 进行绘制，主要流程如下：

* 初始化 Nama SDK
* 道具加载与绑定
* 道具绘制
* 道具的解绑与销毁

### 初始化 Nama SDK

使用 Nama SDK 前，需要先对 Nama SDK 进行初始化。初始化接口说明如下：

```objective-c
/**
 初始化接口3：
     - 初始化SDK，并对 SDK 进行授权，在调用其他接口前必须要先进行初始化。
     - 与 初始化接口2 相比改为通过 v3.bundle 的文件路径进行初始化，并且删除了废弃的 ardata 参数。
 
 @param v3path v3.bundle 对应的文件路径
 @param package 密钥数组，必须配置好密钥，SDK 才能正常工作
 @param size 密钥数组大小
 @param shouldCreate  如果设置为 YES，我们会在内部创建并持有一个 EAGLContext，此时必须使用OC层接口
 @return 初始化结果，为0则初始化失败，大于0则初始化成功
 */
- (int)setupWithDataPath:(NSString *)v3path authPackage:(void *)package authSize:(int)size shouldCreateContext:(BOOL)shouldCreate;
```

示例代码
```objective-c
- (void)initFaceUnity {
    NSString *path = [[NSBundle mainBundle] pathForResource:@"v3.bundle" ofType:nil];
    [[FURenderer shareRenderer] setupWithDataPath:path authPackage:&g_auth_package authSize:sizeof(g_auth_package) shouldCreateContext:YES];
}
```

### 道具加载与绑定

加载风格化形象相关道具时，需要先加载controller道具，然后再加载道具分类中的其他道具，并将这些道具绑定到 controller 道具上（背景道具除外）。道具的加载与绑定相关API如下：

```objective-c
/**
 通过道具文件路径创建道具：
     - 通过道具文件路径创建道具句柄
 
 @param path 道具文件路径
 @return 创建的道具句柄
 */
+ (int)itemWithContentsOfFile:(NSString *)path;

/**
 绑定道具：
     -  该接口可以将一些普通道具绑定到某个目标道具上，从而实现道具间的数据共享，在视频处理时只需要传入该目标道具句柄即可
 
 @param item 目标道具句柄
 @param items 需要被绑定到目标道具上的其他道具的句柄数组
 @param itemsCount 句柄数组包含的道具句柄个数
 @return 被绑定到目标道具上的普通道具个数
 */
+ (int)bindItems:(int)item items:(int*)items itemsCount:(int)itemsCount;
```

### 道具绘制

在绘制风格化形象道具时，首先将 controller 道具及背景道具句柄存储到的一个 int 数组中，然后把该 int 数组作为参数传入 renderBundles 进行绘制即可。相关接口相关API如下：

```objective-c
/**
 道具绘制接口

 @param inPtr 输入数据
 @param inFormat 输入数据格式
 @param outPtr 输出数据
 @param outFormat 输出数据格式
 @param width 图像宽度
 @param height 图像高度
 @param frameid 当前处理的视频帧序数，每次处理完对其进行加 1 操作，不加 1 将无法驱动道具中的特效动画
 @param items 包含多个道具句柄的 int 数组
 @param itemCount 句柄数组中包含的句柄个数
 @return 返回内部纹理ID
 */
- (int)renderBundles:(void *)inPtr inFormat:(FUFormat)inFormat outPtr:(void *)outPtr outFormat:(FUFormat)outFormat width:(int)width height:(int)height frameId:(int)frameid items:(int *)items itemCount:(int)itemCount;

```

在绘制风格化形象的模式下，该接口输入的数据格式为 FU_FORMAT_AVATAR_INFO ，输出的数据为渲染好的图像数据，支持多种主流图像格式，也支持输出纹理ID。在输入格式为FU_FORMAT_AVATAR_INFO，输出的图像宽高可以自定义。接口示例如下：

```objective-c
static int frameid = 0 ;
- (CVPixelBufferRef) renderP2AItemWithPixelBuffer:(CVPixelBufferRef)pixelBuffer RenderMode:(FURenderMode)renderMode Landmarks:(float *)landmarks{
   
    float expression[46] = {0};
    float translation[3] = {0};
    float rotation[4] = {0,0,0,1};
    float rotation_mode[1] = {0};
    float pupil_pos[2] = {0};
    int is_valid = 0 ;
    
    TAvatarInfo info;
    info.p_translation = translation;
    info.p_rotation = rotation;
    info.p_expression = expression;
    info.rotation_mode = rotation_mode;
    info.pupil_pos = pupil_pos;
    info.is_valid = is_valid ;
    
   CVPixelBufferLockBaseAddress(renderTarget, 0);

    void *bytes = (void *)CVPixelBufferGetBaseAddress(renderTarget);
    int stride1 = (int)CVPixelBufferGetBytesPerRow(renderTarget);
	int h1 = (int)CVPixelBufferGetHeight(renderTarget);

	[[FURenderer shareRenderer] renderBundles:&info inFormat:FU_FORMAT_AVATAR_INFO outPtr:pod1 outFormat:FU_FORMAT_BGRA_BUFFER width:stride1/4 height:h1 frameId:frameId ++ items:mItems itemCount:sizeof(mItems)/sizeof(int)];

    
    CVPixelBufferUnlockBaseAddress(renderTarget, 0);

    frameid++;
    return renderTarget ;
}
```

其中renderTarget是用来当作输出的pixelBuffer，创建方式如下：

```objective-c
- (void)createPixelBuffer
{
    CGSize size = [UIScreen mainScreen].currentMode.size;
    if (size.width > 850) {
        
        CGFloat a = 0.7;
        CGFloat w = (((int)(size.width*a) + 3)>>2) * 4;
        CGFloat h = (((int)(size.height*a) + 3)>>2) * 4;
        size = CGSizeMake(w, h);
    }
    
    if (!renderTarget) {
        NSDictionary* pixelBufferOptions = @{ (NSString*) kCVPixelBufferPixelFormatTypeKey :
                                                  @(kCVPixelFormatType_32BGRA),
                                              (NSString*) kCVPixelBufferWidthKey : @(size.width),
                                              (NSString*) kCVPixelBufferHeightKey : @(size.height),
                                              (NSString*) kCVPixelBufferOpenGLESCompatibilityKey : @YES,
                                              (NSString*) kCVPixelBufferIOSurfacePropertiesKey : @{}};
        
        CVPixelBufferCreate(kCFAllocatorDefault,
                            size.width, size.height,
                            kCVPixelFormatType_32BGRA,
                            (__bridge CFDictionaryRef)pixelBufferOptions,
                            &renderTarget);
    }
}
```

### 道具的解绑与销毁

当要切换或去除道具效果时，需先解绑并销毁已绑定的同类型道具，然后再加载绑定新的道具。相关API如下：

```objective-c
/**
 解绑道具：
 -  该接口可以将一些普通道具从某个目标道具上解绑
 
 @param item 目标道具句柄
 @param items 需要从目标道具上解除绑定的普通道具的句柄数组
 @param itemsCount 句柄数组包含的道具句柄个数
 @return 被绑定到目标道具上的普通道具个数
 */
+ (int)unBindItems:(int)item items:(int *)items itemsCount:(int)itemsCount;

/**
 解绑所有道具：
     - 该接口可以解绑绑定在目标道具上的全部道具
 
 @param item 目标道具句柄
 @return 从目标道具上解除绑定的普通道具个数
 */
+ (int)unbindAllItems:(int)item;

/**
 销毁单个道具：
     - 通过道具句柄销毁道具，并释放相关资源
     - 销毁道具后请将道具句柄设为 0 ，以避免 SDK 使用无效的句柄而导致程序出错。
 
 @param item 道具句柄
 */
+ (void)destroyItem:(int)item;
```

### 道具切换与销毁示例

```objective-c
// 加载道具
- (void)loadItemWithtype:(FUItemType)itemType Name:(NSString *)itemName {
    if ([itemName isEqualToString:@"noitem"] || itemName == nil ) {
        // 销毁此道具
        [self destroyItemWithType:itemType];
        return ;
    }
    
    NSString *itemPath = [self bundlePathWithName:itemName];
    
    //创建道具
    int tmpHandle = [FURenderer itemWithContentsOfFile:itemPath];
    
    // 销毁已加载的同类道具
    [self destroyItemWithType:itemType];
    
    // 如果是controller或背景道具，则将道具句柄存放在道具数组中。
    if (itemType == FUItemTypeController || itemType == FUItemTypeBackground) {
        mItems[itemType] = tmpHandle;
    }else{
        // 如果是普通道具，则绑定到controller上
        fuBindItems(mItems[FUItemTypeController], &tmpHandle, 1) ;
    }
}

// 销毁某个道具
- (void)destroyItemWithType:(FUItemType)itemType {
    
    if (mItems[itemType] != 0) {
        
        // 解绑
        if (mItems[FUItemTypeController] && itemType > 2) {
            int tmp[1];
            tmp[0]  = mItems[itemType];
            fuUnbindItems(mItems[FUItemTypeController], tmp, 1) ;
        }
        // 销毁
        [FURenderer destroyItem:mItems[itemType]];
        mItems[itemType] = 0;
    }
}
```
## 调整形象位置
可以通过向nama的target_position传递一个3个元素的数组来设置形象的位置，这个三个元素分别表示形象在x、y、z轴的位置，示例代码如下：
```objective-c
/**
 将Avatar的位置设置为初始状态
 */
- (void)resetScaleToOriginal {
	double position[3] = {
	 0,    // 为形象在x轴的位置
	 0,    // 为形象在y轴的位置
	 0     // 为形象在z轴的位置
	};
	[FURenderer itemSetParamdv:items[FUItemTypeController] withName:@"target_position" value:position length:3];
	[FURenderer itemSetParam:items[FUItemTypeController] withName:@"reset_all" value:@(0.1)];
}
```

## 形象驱动

形象驱动是指使用 Nama SDK 进行人脸检测，再使用检测到人脸信息驱动风格化形象的功能。流程为：先对人脸进行检测，将获取到人脸信息保存到TAvatarInfo结构体中，再将 TAvatarInfo 作为参数传入 renderItems 接口即可，相关API说明如下：

```objective-c
/**
 人脸信息跟踪：
     - 该接口只对人脸进行检测，如果程序中没有运行过视频处理接口( 视频处理接口8 除外)，则需要先执行完该接口才能使用 获取人脸信息接口 来获取人脸信息
 
 @param inputFormat 输入图像格式：FU_FORMAT_BGRA_BUFFER 或 FU_FORMAT_NV12_BUFFER
 @param inputData 输入的图像 bytes 地址
 @param width 图像宽度
 @param height 图像高度
 @return 检测到的人脸个数，返回 0 代表没有检测到人脸
 */
+ (int)trackFace:(int)inputFormat inputData:(void*)inputData width:(int)width height:(int)height;

/**
 获取人脸信息：
     - 在程序中需要先运行过视频处理接口( 视频处理接口8 除外)或 人脸信息跟踪接口 后才能使用该接口来获取人脸信息；
     - 该接口能获取到的人脸信息与我司颁发的证书有关，普通证书无法通过该接口获取到人脸信息；
     - 具体参数及证书要求如下：
 
         landmarks: 2D人脸特征点，返回值为75个二维坐标，长度75*2
         证书要求: LANDMARK证书、AVATAR证书
 
         landmarks_ar: 3D人脸特征点，返回值为75个三维坐标，长度75*3
         证书要求: AVATAR证书
 
         rotation: 人脸三维旋转，返回值为旋转四元数，长度4
         证书要求: LANDMARK证书、AVATAR证书
 
         translation: 人脸三维位移，返回值一个三维向量，长度3
         证书要求: LANDMARK证书、AVATAR证书
 
         eye_rotation: 眼球旋转，返回值为旋转四元数,长度4
         证书要求: LANDMARK证书、AVATAR证书
 
         rotation_raw: 人脸三维旋转（不考虑屏幕方向），返回值为旋转四元数，长度4
         证书要求: LANDMARK证书、AVATAR证书
 
         expression: 表情系数，长度46
         证书要求: AVATAR证书
 
         projection_matrix: 投影矩阵，长度16
         证书要求: AVATAR证书
 
         face_rect: 人脸矩形框，返回值为(xmin,ymin,xmax,ymax)，长度4
         证书要求: LANDMARK证书、AVATAR证书
 
         rotation_mode: 人脸朝向，0-3分别对应手机四种朝向，长度1
         证书要求: LANDMARK证书、AVATAR证书
 
 @param faceId 被检测的人脸 ID ，未开启多人检测时传 0 ，表示检测第一个人的人脸信息；当开启多人检测时，其取值范围为 [0 ~ maxFaces-1] ，取其中第几个值就代表检测第几个人的人脸信息
 @param name 人脸信息参数名： "landmarks" , "eye_rotation" , "translation" , "rotation" ....
 @param pret 作为容器使用的 float 数组指针，获取到的人脸信息会被直接写入该 float 数组。
 @param number float 数组的长度
 @return 返回 1 代表获取成功，返回 0 代表获取失败
 */
+ (int)getFaceInfo:(int)faceId name:(NSString *)name pret:(float *)pret number:(int)number;
```

示例代码如下：

```objective-c
	float expression[46] = {0};
    float translation[3] = {0};
    float rotation[4] = {0,0,0,1};
    float rotation_mode[1] = {0};
    float pupil_pos[2] = {0};
    int is_valid = 0 ;
    
    if (renderMode == FURenderPreviewMode) {
        CVPixelBufferLockBaseAddress(pixelBuffer, 0) ;
        
        void *baseAddress = CVPixelBufferGetBaseAddress(pixelBuffer) ;
        int height = (int)CVPixelBufferGetHeight(pixelBuffer) ;
        int stride = (int)CVPixelBufferGetBytesPerRow(pixelBuffer) ;
        
        [FURenderer trackFace:FU_FORMAT_BGRA_BUFFER inputData:baseAddress width:stride/4 height:height];
        [FURenderer getFaceInfo:0 name:@"expression" pret:expression number:46];
        [FURenderer getFaceInfo:0 name:@"translation" pret:translation number:3];
        [FURenderer getFaceInfo:0 name:@"rotation" pret:rotation number:4];
        [FURenderer getFaceInfo:0 name:@"rotation_mode" pret:rotation_mode number:1];
        [FURenderer getFaceInfo:0 name:@"pupil_pos" pret:pupil_pos number:2];
        
        [FURenderer getFaceInfo:0 name:@"landmarks" pret:landmarks number:150];
        CVPixelBufferUnlockBaseAddress(pixelBuffer, 0) ;
        
        is_valid = [FURenderer isTracking];
    }

    TAvatarInfo info;
    info.p_translation = translation;
    info.p_rotation = rotation;
    info.p_expression = expression;
    info.rotation_mode = rotation_mode;
    info.pupil_pos = pupil_pos;
    info.is_valid = is_valid ;
```

## 形象编辑

形象编辑功能包括：美型，以及对肤色、唇色、瞳色、发型、胡子、眼镜、帽子、衣服的个性化编辑。

- 通过修改 controller.bundle 的相关参数，可以实现美型、及对肤色、唇色、瞳色、发色、胡子颜色、眼镜颜色、帽子颜色的修改。详情请参考[controller说明文档](Controller%20%E8%AF%B4%E6%98%8E%E6%96%87%E6%A1%A3.md)。
- 通过加载并绑定相关道具到 controller.bundle 道具上，可以对发型、胡子、眼镜、帽子、衣服的样式进行修改。详情请参考[道具加载与绑定](#道具加载与绑定)。

在保存形象时，仅有美型功能需要使用 FUP2AClient SDK 的接口生成新的头道具，而其他参数值及道具（发型、胡子、眼镜、帽子、衣服）信息需要客户端缓存。

## 身体驱动

初始化身体驱动功能

```objective-c
NSData *human3dData = [NSData dataWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"human3d.bundle" ofType:nil]];
_human3dPtr = [FURenderer create3DBodyTracker:(void*)human3dData.bytes size:(int)human3dData.length];
```

加载手指驱动，用于手指识别，全局只需要添加一次

```objective-c
    NSString * anim_fistPath = [[NSBundle mainBundle] pathForResource:@"anim_fist.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_fistPath];
    NSString * anim_mergePath = [[NSBundle mainBundle] pathForResource:@"anim_merge.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_mergePath];
    NSString * anim_palmPath = [[NSBundle mainBundle] pathForResource:@"anim_palm.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_palmPath];
    NSString * anim_twoPath = [[NSBundle mainBundle] pathForResource:@"anim_two.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_twoPath];
    NSString * anim_heartPath = [[NSBundle mainBundle] pathForResource:@"anim_heart.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_heartPath];
    NSString * anim_onePath = [[NSBundle mainBundle] pathForResource:@"anim_one.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_onePath];
    NSString * anim_sixPath = [[NSBundle mainBundle] pathForResource:@"anim_six.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_sixPath];
    
    NSString * anim_eightPath = [[NSBundle mainBundle] pathForResource:@"anim_eight.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_eightPath];
    NSString * anim_okPath = [[NSBundle mainBundle] pathForResource:@"anim_ok.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_okPath];
    NSString * anim_thumbPath = [[NSBundle mainBundle] pathForResource:@"anim_thumb.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_thumbPath];
    NSString * anim_holdPath = [[NSBundle mainBundle] pathForResource:@"anim_hold.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_holdPath];
    NSString * anim_korheartPath = [[NSBundle mainBundle] pathForResource:@"anim_korheart.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_korheartPath];
    NSString * anim_rockPath = [[NSBundle mainBundle] pathForResource:@"anim_rock.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_rockPath];
    
    NSString * anim_greetPath = [[NSBundle mainBundle] pathForResource:@"anim_greet.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_greetPath];
    NSString * anim_gunPath = [[NSBundle mainBundle] pathForResource:@"anim_gun.bundle" ofType:nil];
    [self bindItemToControllerWithFilepath:anim_gunPath];
```



开启身体驱动方法（身体驱动分为半身驱动和全身驱动两种模式）,下列方法均在`FUAvatar`类中

```objective-c
/**
 进入身体追踪模式
 */
- (void)enterTrackBodyMode {
    [FURenderer itemSetParam:items[FUItemTypeController] withName:@"enter_human_pose_track_mode" value:@(1)];
}

```

开启半身驱动

```objective-c
//半身驱动方法
- (void)loadHalfAvatar 
{
    fuItemSetParamd(items[FUItemTypeController],"human_3d_track_set_scene",0);
}
/**
 缩放至半身
 */
- (void)resetScaleToHalfBodyWithToolBar
{
    double position[3] = {0,15,-300};
    [FURenderer itemSetParamdv:items[FUItemTypeController] withName:@"target_position" value:position length:3];
    [FURenderer itemSetParam:items[FUItemTypeController] withName:@"reset_all" value:@(6)];
}
```

开启全身驱动

```objective-c
// 加载全身方法
- (void)loadFullAvatar 
{
    fuItemSetParamd(items[FUItemTypeController],"human_3d_track_set_scene",1);
}
/**
 缩放至全身追踪,驱动页未收起模型选择栏等工具栏的情况

 */
- (void)resetScaleToTrackBodyWithToolBar
{
    double position[3] = {0,75,-700};
    [FURenderer itemSetParamdv:items[FUItemTypeController] withName:@"target_position" value:position length:3];
    [FURenderer itemSetParam:items[FUItemTypeController] withName:@"reset_all" value:@(6)];
}
```

关闭身体驱动

```objective-c
/**
 退出身体追踪模式
 */
- (void)quitTrackBodyMode 
{
    [FURenderer itemSetParam:items[FUItemTypeController] withName:@"quit_human_pose_track_mode" value:@(1)];
}
```

进入身体跟随模式

```objective-c
/**
 进入身体跟随模式
 */
- (void)enterFollowBodyMode {
    [FURenderer itemSetParam:items[FUItemTypeController] withName:@"human_3d_track_is_follow" value:@(1)];
}
```

 退出身体跟随模式
 
```objective-c
/**
 退出身体跟随模式
 */
- (void)quitFollowBodyMode {
    [FURenderer itemSetParam:items[FUItemTypeController] withName:@"human_3d_track_is_follow" value:@(0)];
}
```

身体驱动图像处理方法
```
- (CVPixelBufferRef)renderBodyTrackWithBuffer:(CVPixelBufferRef)pixelBuffer ptr:(void *)human3dPtr RenderMode:(FURenderMode)renderMode
{
    dispatch_semaphore_wait(signal, DISPATCH_TIME_FOREVER);
    
    FUAvatarInfo* info=[self GetAvatarInfo:pixelBuffer renderMode:renderMode];
    
    int h = (int)CVPixelBufferGetHeight(pixelBuffer);
    int stride = (int)CVPixelBufferGetBytesPerRow(pixelBuffer);
    int w = stride/4;
    

    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    void* pod = (void *)CVPixelBufferGetBaseAddress(pixelBuffer);
    
    [FURenderer run3DBodyTracker:human3dPtr humanHandle:0 inPtr:pod inFormat:FU_FORMAT_BGRA_BUFFER w:w h:h rotationMode:0];
    
    [[FURenderer shareRenderer]renderBundles:&info->info inFormat:FU_FORMAT_AVATAR_INFO outPtr:pod outFormat:FU_FORMAT_BGRA_BUFFER width:w height:h frameId:frameId++ items:mItems itemCount:sizeof(mItems)/sizeof(int)];
    
    [self rotateImage:pod inFormat:FU_FORMAT_BGRA_BUFFER w:w h:h rotationMode:FU_ROTATION_MODE_0 flipX:NO flipY:YES];
    memcpy(pod, self.rotatedImageManager.mData, w*h*4);
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    dispatch_semaphore_signal(signal);
    
    return pixelBuffer;
}
```

 ## 本地视频驱动“身体驱动”功能

使用本地视频驱动“身体驱动”功能 是指，在驱动界面里面的“身体驱动”子界面里面，通过视频作为输入源来驱动身体驱动功能，与之相对应的是通过相机作为输入源，上面已经介绍。

1.加载本地视频作为输入源
 ```objective-c
#pragma mark ==============================  播放 本地 视频  ====================================
-(void)selectedVideo:(PHAsset *)videoAsset{
	self.asset = videoAsset;
	if (self.asset)
	{
		// 其他代码
		__weak FUBodyTrackController * weakSelf = self;
		[[PHImageManager defaultManager] requestAVAssetForVideo:self.asset options:nil resultHandler:^(AVAsset *avAsset, AVAudioMix *audioMix, NSDictionary *info) {
			NSURL *url = (NSURL *)[[(AVURLAsset *)avAsset URL] fileReferenceURL];
			NSLog(@"url = %@", [url absoluteString]);
			NSLog(@"url = %@", [url relativePath]);
            AVURLAsset *mAsset = (AVURLAsset *)avAsset;
			NSArray * tracks = [mAsset tracksWithMediaType:AVMediaTypeVideo];
			weakSelf.urlAsset = mAsset;
			AVAssetTrack *mTrack = [tracks objectAtIndex:0];
			CGSize size = mTrack.naturalSize;
			UIImageOrientation videoAssetOrientation  = UIImageOrientationUp;
			CGAffineTransform firstTransform = [mTrack preferredTransform];
			if(firstTransform.a == 0 && firstTransform.b == 1.0 && firstTransform.c == -1.0 && firstTransform.d == 0)
			{
				videoAssetOrientation= UIImageOrientationRight;
			}
			if(firstTransform.a == 0 && firstTransform.b == -1.0 && firstTransform.c == 1.0 && firstTransform.d == 0)
			{
				videoAssetOrientation =  UIImageOrientationLeft;
			}
			if(firstTransform.a == 1.0 && firstTransform.b == 0 && firstTransform.c == 0 && firstTransform.d == 1.0)
			{
				videoAssetOrientation =  UIImageOrientationUp;
			}
			if(firstTransform.a == -1.0 && firstTransform.b == 0 && firstTransform.c == 0 && firstTransform.d == -1.0)
			{
				videoAssetOrientation = UIImageOrientationDown;
			}
			weakSelf.currentLoacalVideoOrientation = videoAssetOrientation;
			
			CGRect videoRect = CGRectMake(0.0, 0.0, size.width, size.height);
			videoRect = CGRectApplyAffineTransform(videoRect,firstTransform);
			if (videoRect.size.height > videoRect.size.width)
			{
				// 竖屏视频
				NSLog(@"Portrait mode");
				[weakSelf sethPreViewOrientation:YES];
			}
			else if (videoRect.size.height < videoRect.size.width)
			{
				// 横屏视频
				NSLog(@"Landscape mode");
				[weakSelf sethPreViewOrientation:NO];
			}
			else
			{
				NSLog(@"Square mode");
			}
			[weakSelf playLocalVideoWithTimer];
		}];
	}
	else
	{
		[self.camera startCapture];
	}
}
```
2.使用 CADisplayLink 对象来驱动本地视频，保证视频输出帧率的稳定性


```objective-c
static int FrameNum = 0;
-(void)playLocalVideoWithTimer
{
	FrameNum = 0;
	__weak typeof(self)weakSelf = self;
	dispatch_async(dispatch_get_main_queue(), ^{
		CADisplayLink * displayLink = [CADisplayLink displayLinkWithTarget:weakSelf selector:@selector(displayLinkMethod)];
		// 默认30，后面会根据视频的实际帧率进行调整
		if (@available(iOS 10.0, *)) {
			displayLink.preferredFramesPerSecond = 30;
		} else {
			// Fallback on earlier versions
			displayLink.frameInterval = 1/30.0;
		}
		[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		weakSelf.displayLink = displayLink;
		
		NSError * error;
		AVAssetReader * mReader = [[AVAssetReader alloc] initWithAsset:weakSelf.urlAsset error:&error];
		NSLog(@"加载视频资源出错----------------------%@",error);
		NSArray * tracks = [weakSelf.urlAsset tracksWithMediaType:AVMediaTypeVideo];
		AVAssetTrack *mTrack = [tracks objectAtIndex:0];
		NSDictionary * settingsDic = @{(id)kCVPixelBufferIOSurfacePropertiesKey : [NSDictionary dictionary],(NSString*)kCVPixelBufferPixelFormatTypeKey:[NSNumber numberWithUnsignedInt:kCVPixelFormatType_32BGRA]};
		
		AVAssetReaderTrackOutput * mOutput = [[AVAssetReaderTrackOutput alloc]
											  initWithTrack:mTrack outputSettings:settingsDic];
		mOutput.alwaysCopiesSampleData = NO;
		weakSelf.mOutput = mOutput;
		[mReader addOutput:weakSelf.mOutput];
		weakSelf.mReader = mReader;
		[mReader startReading];
	});
}


```
3.CADisplayLink 对象的驱动方法


```objective-c
-(void)displayLinkMethod
{
    CMSampleBufferRef sampleBuffer = [self.mOutput copyNextSampleBuffer];
    // 其他代码
    if (sampleBuffer)
    {
        
        // Do something with sampleBuffer here.
        [self renderVideoSampleBuffer:sampleBuffer];
        sampleBuffer = NULL;
    }
   // 其他代码
}
```

4.使用nama的渲染方法，渲染出身体驱动的画面，并显示到控件上。

```objective-c
static int frameIndex = 0 ;
-(void)renderVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer
{
    dispatch_semaphore_wait(signal, DISPATCH_TIME_FOREVER);
	__weak typeof(self)weakSelf = self;
	frameIndex = frameIndex +1 ;
	__block CVPixelBufferRef pixelBuffer;
	//pixelBuffer =  CMSampleBufferGetImageBuffer(sampleBuffer);
	switch (self.currentLoacalVideoOrientation) {
		case UIImageOrientationUp:
			pixelBuffer = [FURotation correctBufferOrientation:sampleBuffer withRotationConstant:0];
			break;
		case UIImageOrientationDown:
			pixelBuffer = [FURotation correctBufferOrientation:sampleBuffer withRotationConstant:2];
			break;
		case UIImageOrientationLeft:
			pixelBuffer = [FURotation correctBufferOrientation:sampleBuffer withRotationConstant:1];
			break;
		case UIImageOrientationRight:
			pixelBuffer = [FURotation correctBufferOrientation:sampleBuffer withRotationConstant:3];
			break;
			
		default:
			pixelBuffer = [FURotation correctBufferOrientation:sampleBuffer withRotationConstant:0];
			break;
	}
	
	BOOL bodySwitch = [[[NSUserDefaults standardUserDefaults]valueForKey:@"bodySwitch"] boolValue];
	if (!bodySwitch&& frameIndex == 3)
	{
		[self.currentAvatar loadHalfAvatar];
		[self resetCam];
	}
    const int landmarks_cnt = 150;
    float landmarks[landmarks_cnt] ;
	CVPixelBufferRef buffer = [[FUManager shareInstance]renderBodyTrackAdjustAssginOutputSizeWithBuffer:pixelBuffer ptr:[FUManager shareInstance]->_human3dPtr RenderMode:self.renderMode Landmarks:landmarks LandmarksLength:landmarks_cnt];
	[weakSelf.renderView displayPixelBuffer:buffer withLandmarks:nil count:landmarks_cnt Mirr:NO];
	// ShouldSpreadScreen:NO 代表不铺满全屏，根据视频尺寸不同，可能或显示黑边
	[weakSelf.preView displayPixelBuffer:pixelBuffer withLandmarks:landmarks count:landmarks_cnt bufferMirr:NO landmarksMirr:YES ShouldSpreadScreen:NO];
	CVPixelBufferRelease(pixelBuffer);
	// 销毁 sampleBuffer
	switch (self.currentLoacalVideoOrientation) {
		case UIImageOrientationUp:
			break;
		case UIImageOrientationDown:
			CFRelease(sampleBuffer);
			break;
		case UIImageOrientationLeft:
			CFRelease(sampleBuffer);
			break;
		case UIImageOrientationRight:
			CFRelease(sampleBuffer);
			break;
			
		default:
			break;
	}
	dispatch_semaphore_signal(signal);
}

```

## 文字驱动
文字驱动是指，用文字驱动形象，将输入文字的文字通过FUStaLite.framework获取口型系数转换为表情系数来驱动形象。示例如下：

(1)打开形象的口型系数驱动功能并设置口型系数的权重；
```objective-c
	    // 打开 avatar的口型系数驱动功能
		[self.currentAvatar enableBlendshape];
		// 设置口型系数的权重 expression_weight0、expression_weight1
		NSData *jsonData = [[NSString stringWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"sta_bs_blend_weight" ofType:@"json"] encoding:NSUTF8StringEncoding error:nil] dataUsingEncoding:NSUTF8StringEncoding];
		NSDictionary *jsonDic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:nil];
		double expression_weight0Expression[57] = {0};
		double expression_weight1Expression[57] = {0};
		NSArray * expression_weight0Array = jsonDic[@"expression_weight0"];
		NSArray * expression_weight1Array = jsonDic[@"expression_weight1"];
		for (int i = 0; i < expSize; i++) {
			expression_weight0Expression[i] = [expression_weight0Array[i] doubleValue];
			expression_weight1Expression[i] = [expression_weight1Array[i] doubleValue];
		}
		[self.currentAvatar setExpression_wieght0:expression_weight0Expression];
		[self.currentAvatar setExpression_wieght1:expression_weight1Expression];
 ```
(2)通过FUStaLite.framework获取口型系数；
```objective-c
[[FUStaLiteRequestManager shareManager] process:text
										  voiceName:self.currentToneName
										voiceFormat:@"mp3"
										voiceVolume:@"0.1"
										 voiceSpeed:@"1"
									voiceSamplerate:nil
											 result:^(NSError * _Nullable error, NSData * _Nonnull voiceData, NSData * _Nonnull expressionData,float timeStride) {

 }];
 ```
 (3)将获取的口型系数传至nama驱动形象表情；
 ```objective-c
		double exp[57] = {0.0};
		float *expression = &self.staTotalExpressions[expSize * staFrameIndex];
		for (int i = 0; i < expSize; i++) {
			exp[i] = (double)expression[i];
		}
		[self.currentAvatar setBlend_expression:exp];
 ```


## AR驱动

AR驱动是指，在AR环境下，使用 Nama SDK 进行人脸检测，再使用检测到人脸信息驱动风格化形象。同时我们在AR环境下，支持丰富多彩的滤镜效果。
 ```objective-c
		// 1.即将进入AR滤镜，加载处理头发的道具
		[[FUManager shareInstance] bindHairMask];
		// 2.解绑定身体、上衣、裤子、鞋子资源，只保留头部的一些素材
	    [[FUManager shareInstance] reloadRenderAvatarInARModeInSameController:self.currentAvatar];
	    // 3.设置AR滤镜的controller句柄为arItems[0]
		[[FUManager shareInstance] enterARMode];
		// 4.去除背景道具
		[[FUManager shareInstance] reloadBackGroundAndBindToController:nil];
		// 5.向nama设置enter_ar_mode为1，进入AR滤镜模式
		[self.currentAvatar enterARMode];
```

在AR滤镜模式下，首先将 controller 道具 int 数组中，然后把该 int 数组作为参数传入 renderItems 进行绘制即可。相关接口相关API如下：

```objective-c
/**
 道具绘制接口

 @param inPtr 输入数据
 @param inFormat 输入数据格式
 @param outPtr 输出数据
 @param outFormat 输出数据格式
 @param width 图像宽度
 @param height 图像高度
 @param frameid 当前处理的视频帧序数，每次处理完对其进行加 1 操作，不加 1 将无法驱动道具中的特效动画
 @param items 包含多个道具句柄的 int 数组
 @param itemCount 句柄数组中包含的句柄个数
 @param flip 道具镜像使能，如果设置为 YES 可以将道具做镜像操作
 @return 返回内部纹理ID
 */
- (int)renderItems:(void *)inPtr inFormat:(FUFormat)inFormat outPtr:(void *)outPtr outFormat:(FUFormat)outFormat width:(int)width height:(int)height frameId:(int)frameid items:(int *)items itemCount:(int)itemCount flipx:(BOOL)flip;

```

在AR滤镜模式下，该接口输入的数据格式为 FU_FORMAT_BGRA_BUFFER ，输出的数据为渲染好的图像数据，支持多种主流图像格式，也支持输出纹理ID。

```objective-c
static int frameid = 0 ;
/**
 AR 滤镜处理接口
 
 @param pixelBuffer 图像数据
 @return            处理之后的图像数据
 */
- (CVPixelBufferRef)renderARFilterItemWithBuffer:(CVPixelBufferRef)pixelBuffer{
   
 
	CVPixelBufferRef mirrored_pixel = NULL;
	NSDictionary* pixelBufferOptions = @{ (NSString*) kCVPixelBufferPixelFormatTypeKey :
											  @(kCVPixelFormatType_32BGRA),
										  (NSString*) kCVPixelBufferWidthKey : @(w),
										  (NSString*) kCVPixelBufferHeightKey : @(h),
										  (NSString*) kCVPixelBufferOpenGLESCompatibilityKey : @YES,
										  (NSString*) kCVPixelBufferIOSurfacePropertiesKey : @{}};
	CVPixelBufferCreate(kCFAllocatorDefault,
						w, h,
						kCVPixelFormatType_32BGRA,
						(__bridge CFDictionaryRef)pixelBufferOptions,
						&mirrored_pixel);
    	CVPixelBufferLockBaseAddress(mirrored_pixel, 0);
	void* pod1 = (void *)CVPixelBufferGetBaseAddress(mirrored_pixel);


    void *bytes = (void *)CVPixelBufferGetBaseAddress(renderTarget);
    int stride1 = (int)CVPixelBufferGetBytesPerRow(renderTarget);
	int h1 = (int)CVPixelBufferGetHeight(renderTarget);

	[[FURenderer shareRenderer] renderItems:bytes inFormat:FU_FORMAT_BGRA_BUFFER outPtr:pod1 outFormat:FU_FORMAT_BGRA_BUFFER width:stride/4 height:height frameId:ARFilterID ++ items:arItems itemCount:2 flipx:NO];
    CVPixelBufferUnlockBaseAddress(mirrored_pixel, 0);
	CVPixelBufferRelease(mirrored_pixel);
    frameid++;
    return mirrored_pixel ;
}
```
## 添加脚下阴影
即为形象的左脚和右脚分别添加脚下阴影
```objective-c
/**
 为形象的左脚和右脚分别添加脚下阴影
 */
- (void)bindPlaneShadow {
	if (zuojiao_plane_mg_ptr <= 0) {
		NSString *zuojiao_plane_mg_Path = [[NSBundle mainBundle] pathForResource:@"zuojiao_plane_shadow.bundle" ofType:nil];
		zuojiao_plane_mg_ptr = [self bindItemToControllerWithFilepath:zuojiao_plane_mg_Path];
	}
	if (youjiao_plane_mg_ptr <= 0) {
		NSString *youjiao_plane_mg_Path = [[NSBundle mainBundle] pathForResource:@"youjiao_plane_shadow.bundle" ofType:nil];
		youjiao_plane_mg_ptr = [self bindItemToControllerWithFilepath:youjiao_plane_mg_Path];
	}
}
```
在 FUP2A/FUP2A/ViewController.m 文件中调用

```objective-c
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [[FUManager shareInstance] bindPlaneShadow];
    // 其他代码
}
```

解绑形象的脚下阴影
```objective-c
/**
 分别解绑形象的左脚和右脚的脚下阴影
*/
- (void)unBindPlaneShadow {
	if (zuojiao_plane_mg_ptr > 0) {
		// 解绑
		[FURenderer unBindItems:_defalutQController items:&zuojiao_plane_mg_ptr itemsCount:1];
		// 销毁
		[FURenderer destroyItem:zuojiao_plane_mg_ptr];
		zuojiao_plane_mg_ptr = 0;
	}
	if (youjiao_plane_mg_ptr > 0) {
		// 解绑
		[FURenderer unBindItems:_defalutQController items:&youjiao_plane_mg_ptr itemsCount:1];
		// 销毁
		[FURenderer destroyItem:youjiao_plane_mg_ptr];
		youjiao_plane_mg_ptr = 0;
	}
}
```
在 FUP2A/FUP2A/FUGroupSelectedController.m 文件中调用

```objective-c
- (void)viewDidLoad {
    [super viewDidLoad];
    // 其他代码
    // 解绑脚下阴影
    [[FUManager shareInstance] unBindPlaneShadow];
    // 其他代码
}
```

## 形象应用
形象应用功能包括：单人场景、多人场景、动画场景。

- 单人场景和多人场景可以分别对单个形象和多个形象进行动作编辑，并保存场景图像到手机系统相册。
- 动画场景可以对形象进行动画编辑，导出形象动画为 GIF动图和MP4视频(GIF动图需要传入的类型参数是FUP2AHelperRecordTypeGIF，MP4视频需要传入的类型参数是FUP2AHelperRecordTypeVideo)，并保存到手机系统相册。 其中录制MP4视频示例代码如下：

(1)准备录制MP4视频；
```objective-c
[[FUP2AHelper shareInstance] startRecordWithType:FUP2AHelperRecordTypeVideo];
```


(2)将nama渲染后的CVPixelBufferRef 作为参数传入[FUP2AHelper shareInstance]对象，作为MP4文件的一帧；
 ```objective-c
[[FUP2AHelper shareInstance] recordBufferWithType:FUP2AHelperRecordTypeVideo buffer:imageBuffer sampleBuffer:sampleBuffer];
```


(3)当当前动画一个循环全部录制完成，调用[FUP2AHelper shareInstance]对象的录制完成方法，获取MP4视频文件路径；
 ```objective-c
			float progress = [avatar getAnimateProgress];   // 获取动画的播放进度
			if (progress > 1) {   // 如果动画的播放进度 大于 1，表示动画一个循环录制完成，获取录制的视频文件路径
				renderMode = GroupSelectedRunModeCommon ;
				__weak typeof(self)weakSelf = self ;
				[[FUP2AHelper shareInstance] stopRecordWithType:FUP2AHelperRecordTypeVideo Completion:^(NSString *retPath) {
					dispatch_async(dispatch_get_main_queue(), ^{
						[weakSelf setNextBtnEnable:YES];
						self->videoPath = retPath;
					});
				}];
			}
 ```

- 单人场景、多人场景和动画场景，除了添加动画bundle外，还需要设置相机动画，设置相机动画和形象的示例代码如下：
 ```objective-c
				// 获取当前相机动画bundle路径
				NSString *camPath = [[NSBundle mainBundle] pathForResource:self.singleModel.camera ofType:@"bundle"];
				// 将相机动画绑定到controller上
				[[FUManager shareInstance] reloadCamItemWithPath:camPath];
				// 循环播放相机动画（这里可以自定义）
				[avatar loopCameraAnimation];
				// 获取当前形象动画bundle路径
				NSString *animationPath = [[NSBundle mainBundle] pathForResource:self.singleModel.animationName ofType:@"bundle"];
				// 将形象动画绑定到controller上
				[avatar reloadAnimationWithPath:animationPath];
 ```




**更多详情，请参考Demo代码!**
