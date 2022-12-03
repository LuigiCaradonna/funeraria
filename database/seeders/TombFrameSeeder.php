<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class TombFrameSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $frames = [
            ['name' => '3x2 a becco'],
            ['name' => '4x2 a becco'],
            ['name' => '5x2 a becco'],
            ['name' => '3x2 a costa dritta'],
            ['name' => '4x2 a costa dritta'],
            ['name' => '5x2 a costa dritta'],
        ];

        DB::table('tomb_frames')->insert($frames);
    }
}
