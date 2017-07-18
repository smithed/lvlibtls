<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="16008000">
	<Property Name="SMProvider.SMVersion" Type="Int">201310</Property>
	<Item Name="My Computer" Type="My Computer">
		<Property Name="IOScan.Faults" Type="Str"></Property>
		<Property Name="IOScan.NetVarPeriod" Type="UInt">100</Property>
		<Property Name="IOScan.NetWatchdogEnabled" Type="Bool">false</Property>
		<Property Name="IOScan.Period" Type="UInt">10000</Property>
		<Property Name="IOScan.PowerupMode" Type="UInt">0</Property>
		<Property Name="IOScan.Priority" Type="UInt">9</Property>
		<Property Name="IOScan.ReportModeConflict" Type="Bool">true</Property>
		<Property Name="IOScan.StartEngineOnDeploy" Type="Bool">false</Property>
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="lvlibtls" Type="Folder">
			<Item Name="client" Type="Folder">
				<Item Name="lvlibretls client socket.lvclass" Type="LVClass" URL="../lvlibtls/client/lvlibretls client socket.lvclass"/>
			</Item>
			<Item Name="context parent" Type="Folder">
				<Item Name="lvlibretls configurable context.lvclass" Type="LVClass" URL="../lvlibtls/context parent/lvlibretls configurable context.lvclass"/>
			</Item>
			<Item Name="crypto digest" Type="Folder">
				<Item Name="lvlibcrypto digest.lvclass" Type="LVClass" URL="../lvlibtls/crypto digest/lvlibcrypto digest.lvclass"/>
			</Item>
			<Item Name="server" Type="Folder">
				<Item Name="lvlibretls server listener.lvclass" Type="LVClass" URL="../lvlibtls/server/lvlibretls server listener.lvclass"/>
			</Item>
			<Item Name="socket connection" Type="Folder">
				<Item Name="lvlibretls connection.lvclass" Type="LVClass" URL="../lvlibtls/socket connection/lvlibretls connection.lvclass"/>
			</Item>
			<Item Name="libcrypto-41.dll" Type="Document" URL="../lvlibtls/libcrypto-41.dll"/>
			<Item Name="libssl-43.dll" Type="Document" URL="../lvlibtls/libssl-43.dll"/>
			<Item Name="libtls-15.dll" Type="Document" URL="../lvlibtls/libtls-15.dll"/>
			<Item Name="lv tls dll.lvlib" Type="Library" URL="../lvlibtls/lv tls dll.lvlib"/>
			<Item Name="lvlibtls.dll" Type="Document" URL="../lvlibtls/lvlibtls.dll"/>
		</Item>
		<Item Name="test" Type="Folder">
			<Item Name="server hello world.vi" Type="VI" URL="../tests/server hello world.vi"/>
			<Item Name="stupid get action.vi" Type="VI" URL="../tests/stupid get action.vi"/>
			<Item Name="stupid put action.vi" Type="VI" URL="../tests/stupid put action.vi"/>
			<Item Name="test responder.lvclass" Type="LVClass" URL="../tests/test responder/test responder.lvclass"/>
			<Item Name="test ws client.vi" Type="VI" URL="../tests/test ws client.vi"/>
			<Item Name="test ws standalone.vi" Type="VI" URL="../tests/test ws standalone.vi"/>
			<Item Name="testclient.vi" Type="VI" URL="../tests/testclient.vi"/>
			<Item Name="Websocket console plugin.lvclass" Type="LVClass" URL="../tests/ws console/Websocket console plugin.lvclass"/>
			<Item Name="ws key accept process.vi" Type="VI" URL="../tests/ws key accept process.vi"/>
		</Item>
		<Item Name="websockets" Type="Folder">
			<Item Name="websocket client.lvclass" Type="LVClass" URL="../websocket/client/websocket client.lvclass"/>
			<Item Name="websocket core.lvlib" Type="Library" URL="../websocket/websocket core.lvlib"/>
			<Item Name="websocket standalone server.lvclass" Type="LVClass" URL="../websocket/standalone server/websocket standalone server.lvclass"/>
			<Item Name="websocket upgrade handler.lvclass" Type="LVClass" URL="../websocket/websocket upgrade handler.lvclass"/>
			<Item Name="websocket user plugin.lvclass" Type="LVClass" URL="../websocket/user plugin/websocket user plugin.lvclass"/>
		</Item>
		<Item Name="basic HTTP handler dataset.lvclass" Type="LVClass" URL="../HTTP handler dataset/basic HTTP handler dataset.lvclass"/>
		<Item Name="Basic HTTP server.lvclass" Type="LVClass" URL="../HTTP server/Basic HTTP server.lvclass"/>
		<Item Name="http headers.lvlib" Type="Library" URL="../HTTP headers/http headers.lvlib"/>
		<Item Name="http responder.lvclass" Type="LVClass" URL="../HTTP responder/http responder.lvclass"/>
		<Item Name="http tls handle content data.vi" Type="VI" URL="../TLS HTTP server/http tls handle content data.vi"/>
		<Item Name="main.vi" Type="VI" URL="../main.vi"/>
		<Item Name="TLS HTTP handler dataset.lvclass" Type="LVClass" URL="../TLS HTTP server/TLS handler dataset/TLS HTTP handler dataset.lvclass"/>
		<Item Name="TLS HTTP server.lvclass" Type="LVClass" URL="../TLS HTTP server/TLS HTTP server.lvclass"/>
		<Item Name="uniform resource locator.lvlib" Type="Library" URL="../URI/uniform resource locator.lvlib"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Check if File or Folder Exists.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/libraryn.llb/Check if File or Folder Exists.vi"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="High Resolution Relative Seconds.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/High Resolution Relative Seconds.vi"/>
				<Item Name="Internecine Avoider.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/tcp.llb/Internecine Avoider.vi"/>
				<Item Name="LabVIEWHTTPClient.lvlib" Type="Library" URL="/&lt;vilib&gt;/httpClient/LabVIEWHTTPClient.lvlib"/>
				<Item Name="NI_FileType.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/lvfile.llb/NI_FileType.lvlib"/>
				<Item Name="NI_PackedLibraryUtility.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/LVLibp/NI_PackedLibraryUtility.lvlib"/>
				<Item Name="NI_WebServices.lvlib" Type="Library" URL="/&lt;vilib&gt;/wsapi/NI_WebServices.lvlib"/>
				<Item Name="Path To Command Line String.vi" Type="VI" URL="/&lt;vilib&gt;/AdvancedString/Path To Command Line String.vi"/>
				<Item Name="PathToUNIXPathString.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/CFURL.llb/PathToUNIXPathString.vi"/>
				<Item Name="Space Constant.vi" Type="VI" URL="/&lt;vilib&gt;/dlg_ctls.llb/Space Constant.vi"/>
				<Item Name="TCP Listen Internal List.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/tcp.llb/TCP Listen Internal List.vi"/>
				<Item Name="TCP Listen List Operations.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/tcp.llb/TCP Listen List Operations.ctl"/>
				<Item Name="TCP Listen.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/tcp.llb/TCP Listen.vi"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
			</Item>
			<Item Name="websocket unmask data.vi" Type="VI" URL="../websocket/websocket unmask data.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="My Application" Type="EXE">
				<Property Name="App_INI_aliasGUID" Type="Str">{38F1C0AA-0796-42B7-9AD3-297D28850137}</Property>
				<Property Name="App_INI_GUID" Type="Str">{DCF3C5A3-B865-4107-8A58-E31EE21D123A}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="App_winsec.description" Type="Str">http://www.ni.com</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{F4BC7C01-7906-411B-B73F-995EC58128ED}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">My Application</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../builds/NI_AB_PROJECTNAME/My Application</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToCommon</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{F65D584F-2122-4412-A060-7886C8AB4567}</Property>
				<Property Name="Bld_version.build" Type="Int">5</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">httplisten.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../builds/NI_AB_PROJECTNAME/My Application/httplisten.exe</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../builds/NI_AB_PROJECTNAME/My Application/data</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{0EE1708F-DF64-411E-AB10-1E4B4952E047}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/main.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">ni</Property>
				<Property Name="TgtF_fileDescription" Type="Str">My Application</Property>
				<Property Name="TgtF_internalName" Type="Str">My Application</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2015 ni</Property>
				<Property Name="TgtF_productName" Type="Str">My Application</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{D17266F3-ED27-4EA6-A258-B188A26ABA41}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">httplisten.exe</Property>
			</Item>
		</Item>
	</Item>
</Project>
