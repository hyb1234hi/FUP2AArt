//
//  FUP2AHelper.h
//  FUP2A
//
//  Created by L on 2019/2/22.
//  Copyright © 2019年 L. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
typedef enum : NSUInteger {
    FUP2AHelperRecordTypeVideo,     // 视频
	FUP2AHelperRecordTypeVoicedVideo,     // 有声视频
	FUP2AHelperRecordTypeStaVideo,     // 有声sta视频
    FUP2AHelperRecordTypeGIF,       // gif
} FUP2AHelperRecordType;

@interface FUP2AHelper : NSObject

@property (nonatomic,strong)NSString * saveVideoPath;    // 保存视频路径  ⚠️⚠️⚠️⚠️⚠️⚠️ 录制视频前必须设置
@property (nonatomic,strong)NSString * saveAudioPath;    // 保存音频路径  ⚠️⚠️⚠️⚠️⚠️⚠️ 录制sta有声视频前必须设置
/**
 FUP2AHelper 单例
 */
+ (instancetype)shareInstance ;

/**
 *  初始化 FUP2AHelper
 - 需要先初始化 data 才能使用其他接口，全局只需要初始化 data 一次
 
 @param package         密钥数组，必须配置好密钥，SDK 才能正常工作
 @param size            密钥数组大小
 */

- (void)setupHelperWithAuthPackage:(void *)package
                          authSize:(int)size;

/**
 视频数据生成平面图片

 @param buffer  视频数据
 @param mirr    是否镜像
 @return        图片
 */
- (UIImage *)createImageWithBuffer:(CVPixelBufferRef)buffer mirr:(BOOL)mirr;

/**
 视频数据生成原始尺寸的平面图片
 
 @param buffer  视频数据
 @param mirr    是否镜像
 @return        图片
 */
- (UIImage *)createOriginSizeImageWithBuffer:(CVPixelBufferRef)buffer mirr:(BOOL)mirr;
/**
 开始视频录制

 @param recordType 录制类型
 */
- (void)startRecordWithType:(FUP2AHelperRecordType)recordType ;

/**
 拼接视频数据

 @param recordType      录制类型
 @param buffer          最后拼接的视频数据
 @param sampleBuffer    原始视频数据
 */
- (void)recordBufferWithType:(FUP2AHelperRecordType)recordType
                      buffer:(CVPixelBufferRef)buffer
                sampleBuffer:(CMSampleBufferRef)sampleBuffer ;
                /**
 拼接视频数据

 @param recordType      录制类型
 @param buffer          最后拼接的视频数据
 @param sampleBuffer    原始视频数据
 @param handle          是录制开始到当前帧的时长的d回调
 */
- (void)recordBufferWithType:(FUP2AHelperRecordType)recordType
                      buffer:(CVPixelBufferRef)buffer
                sampleBuffer:(CMSampleBufferRef)sampleBuffer Completion:(void (^)(CFAbsoluteTime duration))handle;

/**
 停止视频录制

 @param recordType  录制类型
 @param handle      返回录制结果所在路径
 */
- (void)stopRecordWithType:(FUP2AHelperRecordType)recordType
                Completion:(void (^)(NSString *retPath))handle ;

/**
 停止视频录制,并回调时长
 
 @param recordType  录制类型
 @param handle      返回录制结果所在路径，时长
 */
- (void)stopRecordWithType:(FUP2AHelperRecordType)recordType
				TimeCompletion:(void (^)(NSString *retPath,CFAbsoluteTime duration))handle;
/**
 取消录制
 */
- (void)cancleRecord ;

@end
