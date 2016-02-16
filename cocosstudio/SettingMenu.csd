<GameProjectFile>
  <PropertyGroup Type="Layer" Name="SettingMenu" ID="d9c86e40-975c-45e3-88f6-66b33d36ace7" Version="2.3.2.3" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="60" Speed="1.0000" ActivedAnimationName="In">
        <Timeline ActionTag="-1087260518" Property="Position">
          <PointFrame FrameIndex="0" X="320.0000" Y="-365.2489">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="30" X="320.0000" Y="450.0000">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="60" X="320.0000" Y="-365.2489">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="Out" StartIndex="0" EndIndex="30">
          <RenderColor A="150" R="64" G="224" B="208" />
        </AnimationInfo>
        <AnimationInfo Name="In" StartIndex="30" EndIndex="60">
          <RenderColor A="150" R="250" G="235" B="215" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" Tag="32" ctype="GameLayerObjectData">
        <Size X="640.0000" Y="960.0000" />
        <Children>
          <AbstractNodeData Name="Menu" ActionTag="-1087260518" Tag="41" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="64.0000" RightMargin="64.0000" TopMargin="60.0000" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Size X="512.0000" Y="900.0000" />
            <Children>
              <AbstractNodeData Name="FastGameBtn" ActionTag="-1108464738" Tag="42" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="106.0000" RightMargin="106.0000" TopMargin="810.0000" BottomMargin="10.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="32" Scale9Height="43" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="300.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1" ActionTag="-411701770" Tag="44" IconVisible="False" VerticalEdge="BothEdge" LeftMargin="15.0000" RightMargin="141.0000" TopMargin="24.0000" BottomMargin="24.0000" FontSize="24" LabelText="开启快速模式" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="144.0000" Y="32.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="15.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.0500" Y="0.5000" />
                    <PreSize X="0.3100" Y="0.4000" />
                    <FontResource Type="Normal" Path="msyh.ttc" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Sprite_2" ActionTag="-171739056" Tag="46" IconVisible="False" VerticalEdge="BothEdge" LeftMargin="231.0000" RightMargin="15.0000" TopMargin="13.0000" BottomMargin="13.0000" ctype="SpriteObjectData">
                    <Size X="54.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="258.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8600" Y="0.5000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="MarkedSubImage" Path="image/icon_fast.png" Plist="res/image01.plist" />
                    <BlendFunc Src="770" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="256.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.0556" />
                <PreSize X="0.5859" Y="0.0889" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="MarkedSubImage" Path="image/choice_btn_unchosen.9.png" Plist="res/image01.plist" />
                <PressedFileData Type="MarkedSubImage" Path="image/choice_btn_unchosen.9.png" Plist="res/image01.plist" />
                <NormalFileData Type="MarkedSubImage" Path="image/choice_btn.9.png" Plist="res/image01.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="BackToDayBtn" ActionTag="404870603" Tag="43" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="106.0000" RightMargin="106.0000" TopMargin="722.7126" BottomMargin="97.2874" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="32" Scale9Height="43" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="300.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0" ActionTag="-2018905303" Tag="45" IconVisible="False" VerticalEdge="BothEdge" LeftMargin="15.0000" RightMargin="85.0000" TopMargin="8.0000" BottomMargin="8.0000" IsCustomSize="True" FontSize="24" LabelText="要返回故事的某一天吗？" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="200.0000" Y="64.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="15.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.0500" Y="0.5000" />
                    <PreSize X="0.6667" Y="0.8000" />
                    <FontResource Type="Normal" Path="msyh.ttc" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Sprite_2_0" ActionTag="2055919773" Tag="47" IconVisible="False" VerticalEdge="BothEdge" LeftMargin="231.0000" RightMargin="15.0000" TopMargin="13.0000" BottomMargin="13.0000" ctype="SpriteObjectData">
                    <Size X="54.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="258.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8600" Y="0.5000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="MarkedSubImage" Path="image/icon_restart.png" Plist="res/image01.plist" />
                    <BlendFunc Src="770" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="256.0000" Y="137.2874" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1525" />
                <PreSize X="0.5859" Y="0.0889" />
                <FontResource Type="Normal" Path="msyh.ttc" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="MarkedSubImage" Path="image/choice_btn_unchosen.9.png" Plist="res/image01.plist" />
                <PressedFileData Type="MarkedSubImage" Path="image/choice_btn_unchosen.9.png" Plist="res/image01.plist" />
                <NormalFileData Type="MarkedSubImage" Path="image/choice_btn.9.png" Plist="res/image01.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="SlowGameBtn" ActionTag="-139444526" VisibleForFrame="False" Tag="48" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="106.0000" RightMargin="106.0000" TopMargin="810.0000" BottomMargin="10.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="32" Scale9Height="43" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="300.0000" Y="80.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1" ActionTag="-380503365" Tag="49" IconVisible="False" VerticalEdge="BothEdge" LeftMargin="15.0000" RightMargin="141.0000" TopMargin="24.0000" BottomMargin="24.0000" FontSize="24" LabelText="关闭快速模式" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="144.0000" Y="32.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="15.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.0500" Y="0.5000" />
                    <PreSize X="0.3100" Y="0.4000" />
                    <FontResource Type="Normal" Path="msyh.ttc" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Sprite_2" ActionTag="-2027489371" Tag="50" IconVisible="False" VerticalEdge="BothEdge" LeftMargin="231.0000" RightMargin="15.0000" TopMargin="13.0000" BottomMargin="13.0000" ctype="SpriteObjectData">
                    <Size X="54.0000" Y="54.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="258.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8600" Y="0.5000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="MarkedSubImage" Path="image/icon_slow.png" Plist="res/image01.plist" />
                    <BlendFunc Src="770" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="256.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.0556" />
                <PreSize X="0.5859" Y="0.0889" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="MarkedSubImage" Path="image/choice_btn_unchosen.9.png" Plist="res/image01.plist" />
                <PressedFileData Type="MarkedSubImage" Path="image/choice_btn_unchosen.9.png" Plist="res/image01.plist" />
                <NormalFileData Type="MarkedSubImage" Path="image/choice_btn_chosen.9.png" Plist="res/image01.plist" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="320.0000" Y="450.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.4688" />
            <PreSize X="0.8000" Y="0.9375" />
            <FileData Type="Normal" Path="image/bottomareafull.png" Plist="" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>